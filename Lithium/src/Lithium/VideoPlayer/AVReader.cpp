#include "lipch.h"
#include "AVReader.h"

extern "C"
{
#include "libavformat/avformat.h"
}

namespace Li
{
	AVReader::AVReader(const std::string& path)
		: m_FormatCtx(nullptr), m_Packet(nullptr), m_VideoPackets(32, 128), m_AudioPackets(64, 256), m_Finished(false), m_Running(false)
	{
		m_FormatCtx = avformat_alloc_context();
		if (m_FormatCtx == nullptr)
		{
			LI_CORE_ERROR("Failed to allocate memory for libav format context ({})", path);
			throw "Failed to allocate memory for libav format context";
		}
		// Open media file for demuxing.
		int err = avformat_open_input(&m_FormatCtx, path.c_str(), NULL, NULL);
		if (err < 0)
		{
			char errbuf[AV_ERROR_MAX_STRING_SIZE];
			av_strerror(err, errbuf, AV_ERROR_MAX_STRING_SIZE);
			LI_CORE_ERROR("Failed to open video: {} ({})", errbuf, path);
			throw "Failed to open video";
		}
		// Read packets of the media file to get stream info.
		if (avformat_find_stream_info(m_FormatCtx, NULL) < 0)
		{
			char errbuf[AV_ERROR_MAX_STRING_SIZE];
			av_strerror(err, errbuf, AV_ERROR_MAX_STRING_SIZE);
			LI_CORE_ERROR("Failed to find stream info: {} ({})", errbuf, path);
			throw "Failed to find stream info";
		}

		InitVideo(path);
		InitAudio(path);

		m_Packet = av_packet_alloc();
		if (m_Packet == nullptr)
		{
			LI_CORE_ERROR("Failed to allocate memory for packet ({})", path);
			throw "Failed to allocate memory for packet";
		}
	}

	AVReader::~AVReader()
	{
		m_Running = false;
		m_VideoPackets.Stop();
		m_AudioPackets.Stop();
		m_ReadThread.join();

		if (m_FormatCtx)
			avformat_close_input(&m_FormatCtx);
		if (m_Packet)
			av_packet_free(&m_Packet);
	}

	void AVReader::Start()
	{
		m_Running = true;
		m_ReadThread = std::thread(&AVReader::ReadThreadEntry, this);
	}

	void AVReader::InitVideo(const std::string& path)
	{
		// Find index of video stream.
		m_VideoStreamIndex = av_find_best_stream(m_FormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
		if (m_VideoStreamIndex < 0)
		{
			LI_CORE_ERROR("Video stream not found ({})", path);
			throw "Video stream not found in media file";
		}
		// Check that stream is VP9 codec.
		m_VideoStream = m_FormatCtx->streams[m_VideoStreamIndex];
		if (m_VideoStream->codecpar->codec_id != AV_CODEC_ID_VP9)
		{
			LI_CORE_ERROR("Video stream codec must be VP9 ({})", path);
			throw "Video stream codec must be VP9";
		}
	}

	void AVReader::InitAudio(const std::string& path)
	{
		// Find index of audio stream.
		m_AudioStreamIndex = av_find_best_stream(m_FormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);

		m_AudioStream = m_FormatCtx->streams[m_AudioStreamIndex];
		if (m_AudioStream->codecpar->codec_id != AV_CODEC_ID_OPUS)
		{
			LI_CORE_ERROR("Audio stream codec must be OPUS ({})", path);
			m_AudioStreamIndex = -1;
		}
	}

	void AVReader::ReadThreadEntry()
	{
		while (m_Running.load())
		{
			// Read next packet in media file.
			if (av_read_frame(m_FormatCtx, m_Packet) < 0)
				break;

			AVPacket* packet_clone = av_packet_clone(m_Packet);
			if (m_Packet->stream_index == m_VideoStreamIndex)
				m_VideoPackets.Enqueue(packet_clone);
			else if (m_Packet->stream_index == m_AudioStreamIndex)
				m_AudioPackets.Enqueue(packet_clone);

			av_packet_unref(m_Packet);
		}
		m_Finished = true;
	}
}
