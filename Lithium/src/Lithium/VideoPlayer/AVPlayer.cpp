#include "lipch.h"
#include "AVPlayer.h"

#include "Lithium/Core/Assert.h"
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libavutil/opt.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
}

namespace Li
{
	static float RationalToFloat(const AVRational& time_base);
	
	AVPlayer::AVPlayer()
		: m_Open(false), m_Playing(false), m_Elapsed(0), m_Frame(nullptr)
	{
	}

	AVPlayer::~AVPlayer()
	{
		FreeInternal();
	}

	bool AVPlayer::Open(const std::string& path, FilterType min_filter, FilterType mag_filter)
	{
		LI_CORE_ASSERT(!m_Open, "Player already open.");

		try {
			m_Reader = MakeUnique<AVReader>(path);
		}
		catch (...)
		{
			m_Reader.reset();
			return false;
		}

		if (InitVideo(path, min_filter, mag_filter) == false)
			return false;
		// If audio fails, play video without audio.
		m_Audio.HasAudio = m_Reader->HasAudio();
		if (m_Audio.HasAudio)
			m_Audio.HasAudio = InitAudio(path);

		m_Reader->Start();

		m_Frame = av_frame_alloc();
		if (!m_Frame)
		{
			Log::CoreError("Failed to allocate memory for frame ({})", path);
			FreeInternal();
			return false;
		}

		Log::CoreInfo("Opened {}: width={} height={}", path, m_Video.Width, m_Video.Height);
		m_Open = true;
		m_Playing = true;
		m_Elapsed = Duration::us(0);

		return true;
	}

	bool AVPlayer::InitVideo(const std::string& path, FilterType min_filter, FilterType mag_filter)
	{
		m_Video.Width = m_Reader->GetVideoStream()->codecpar->width;
		m_Video.Height = m_Reader->GetVideoStream()->codecpar->height;
		LI_CORE_ASSERT(m_Video.Width <= 4096, "Video width too large!");
		LI_CORE_ASSERT(m_Video.Height <= 4096, "Video height too large!");

		m_Video.Codec = avcodec_find_decoder(AV_CODEC_ID_VP9);
		m_Video.CodecCtx = avcodec_alloc_context3(m_Video.Codec);
		if (!m_Video.CodecCtx)
		{
			Log::CoreError("Failed to allocate memory for video codec context ({})", path);
			FreeInternal();
			return false;
		}
		if (avcodec_parameters_to_context(m_Video.CodecCtx, m_Reader->GetVideoStream()->codecpar) < 0)
		{
			Log::CoreError("Failed to fill video codec parameters ({})", path);
			FreeInternal();
			return false;
		}
		if (avcodec_open2(m_Video.CodecCtx, m_Video.Codec, NULL) < 0)
		{
			Log::CoreError("Failed to open codec through avcodec_open2 ({})", path);
			FreeInternal();
			return false;
		}

		// Used to convert color space to RGBA.
		m_Video.SwsContext = sws_getContext(m_Video.Width, m_Video.Height, m_Video.CodecCtx->pix_fmt, m_Video.Width, m_Video.Height, AV_PIX_FMT_RGBA, 2, NULL, NULL, NULL);
		if (!m_Video.SwsContext)
		{
			Log::CoreError("Failed to create color space context ({})", path);
			FreeInternal();
			return false;
		}
		
		size_t px_buffer_size = (size_t)VideoData::NumOutChannels * m_Video.Width * m_Video.Height;
		m_Video.PixelBuffer = new uint8_t[px_buffer_size];
		memset(m_Video.PixelBuffer, 0, px_buffer_size);

		m_Video.Texture = Texture2D::Create(m_Video.Width, m_Video.Height, VideoData::NumOutChannels, nullptr,
			WrapType::ClampToEdge, WrapType::ClampToEdge, min_filter, mag_filter, true);

		m_Video.TimeBase = RationalToFloat(m_Reader->GetVideoStream()->time_base);
		LI_CORE_ASSERT(m_Video.TimeBase > 0.0f, "Invalid time base.");

		return true;
	}

	bool AVPlayer::InitAudio(const std::string& path)
	{
		m_Audio.Codec = avcodec_find_decoder(AV_CODEC_ID_OPUS);
		if (!m_Audio.Codec)
		{
			FreeAudioInternal();
			Log::CoreError("Failed to find Opus codec ({})", path);
			return false;
		}
		m_Audio.CodecCtx = avcodec_alloc_context3(m_Audio.Codec);
		if (!m_Audio.CodecCtx)
		{
			FreeAudioInternal();
			Log::CoreError("Failed allocate audio codec context ({})", path);
			return false;
		}
		if (avcodec_parameters_to_context(m_Audio.CodecCtx, m_Reader->GetAudioStream()->codecpar) < 0)
		{
			Log::CoreError("Failed to fill audio codec parameters ({})", path);
			FreeInternal();
			return false;
		}
		if (avcodec_open2(m_Audio.CodecCtx, m_Audio.Codec, NULL) < 0)
		{
			FreeAudioInternal();
			Log::CoreError("Failed to open audio codec through avcodec_open2 ({})", path);
			return false;
		}

		m_Audio.Channels = m_Audio.CodecCtx->channels;
		m_Audio.Frequency = m_Audio.CodecCtx->sample_rate;

		// OpenAL wants signed 8/16 bit mono/stereo audio.
		AVSampleFormat format = m_Audio.CodecCtx->sample_fmt;
		m_Audio.Resample = format != AV_SAMPLE_FMT_S16 && format != AV_SAMPLE_FMT_U8;

		// Resample if audio is not in the correct format.
		if (m_Audio.Resample)
		{
			m_Audio.SwrCtx = swr_alloc();
			if (!m_Audio.SwrCtx)
			{
				m_Audio.HasAudio = false;
				FreeAudioInternal();
				Log::CoreError("Failed to allocate memory for audio resampler ({})", path);
				return false;
			}

			av_opt_set_int(m_Audio.SwrCtx, "in_channel_count", m_Audio.Channels, 0);
			av_opt_set_int(m_Audio.SwrCtx, "out_channel_count", m_Audio.Channels, 0);
			av_opt_set_int(m_Audio.SwrCtx, "in_channel_layout", m_Audio.CodecCtx->channel_layout, 0);
			av_opt_set_int(m_Audio.SwrCtx, "out_channel_layout", m_Audio.CodecCtx->channel_layout, 0);
			av_opt_set_int(m_Audio.SwrCtx, "in_sample_rate", m_Audio.Frequency, 0);
			av_opt_set_int(m_Audio.SwrCtx, "out_sample_rate", m_Audio.Frequency, 0);
			av_opt_set_sample_fmt(m_Audio.SwrCtx, "in_sample_fmt", format, 0);
			// if input format is 8 bit make output 8 bit, otherwise use 16 bit.
			if (format == AV_SAMPLE_FMT_U8P)
			{
				m_Audio.OutFormat = AV_SAMPLE_FMT_U8;
				m_Audio.OutBytesPerSample = 1;
			}
			else
			{
				m_Audio.OutFormat = AV_SAMPLE_FMT_S16;
				m_Audio.OutBytesPerSample = 2;
			}
			
			av_opt_set_sample_fmt(m_Audio.SwrCtx, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);

			swr_init(m_Audio.SwrCtx);
			if (!swr_is_initialized(m_Audio.SwrCtx))
			{
				m_Audio.HasAudio = false;
				FreeAudioInternal();
				Log::CoreError("Failed to init audio resampler ({})", path);
				return false;
			}
		}
		m_Audio.DiscardCount = 0;

		return true;
	}

	void AVPlayer::Close()
	{
		m_Video.Texture.reset();
		m_Open = false;
		m_Reader.reset();
		FreeInternal();
	}

	bool AVPlayer::UpdateFrame(Duration::us dt)
	{
		if (!m_Playing)
			return false;

		// Play video when audio queue is ready.
		if (dt < Li::Duration::ms(200) && !m_Audio.HasAudio || (m_Audio.HasAudio && m_Audio.Started))
			m_Elapsed += dt;

		// VIDEO //
		if (m_Elapsed > Li::Duration::fsec(m_Video.NextPTS * m_Video.TimeBase))
		{
			m_Video.Texture->SetData(m_Video.PixelBuffer, m_Video.Width, m_Video.Height, true);

			AVPacket* packet = m_Reader->DequeueVideo();
			if (packet)
			{
				m_Playing = ProcessVideoPacket(packet);
				av_packet_free(&packet);
			}
			else if (m_Reader->IsFinished())
				m_Playing = false;
			else
				Log::CoreWarn("No video packet ready.");
		}

		// AUDIO //
		if (m_Audio.HasAudio && (!m_Audio.Queue || m_Audio.Queue->NeedsAudio()))
		{
			AVPacket* packet = m_Reader->DequeueAudio();
			if (packet)
			{
				m_Playing = ProcessAudioPacket(packet);

				// Play media when audio queue is full.
				if (m_Playing && !m_Audio.Queue->IsPlaying() && !m_Audio.Queue->NeedsAudio())
				{
					m_Audio.Queue->Play();
					m_Audio.Started = true;
				}

				av_packet_free(&packet);
			}
			else if (!m_Reader->IsFinished())
				Log::CoreWarn("No audio packet ready.");
		}

		return m_Playing;
	}

	void AVPlayer::FreeInternal()
	{
		FreeAudioInternal();

		if (m_Video.PixelBuffer)
		{
			delete m_Video.PixelBuffer;
			m_Video.PixelBuffer = nullptr;
		}
		if (m_Video.SwsContext)
		{
			sws_freeContext(m_Video.SwsContext);
			m_Video.SwsContext = nullptr;
		}
		if (m_Frame)
			av_frame_free(&m_Frame);
		if (m_Video.CodecCtx)
			avcodec_free_context(&m_Video.CodecCtx);
	}

	void AVPlayer::FreeAudioInternal()
	{
		m_Audio.Queue.reset();

		if (m_Audio.DstData)
		{
			av_freep(&m_Audio.DstData[0]);
			av_freep(&m_Audio.DstData);
		}

		if (m_Audio.SwrCtx)
			swr_free(&m_Audio.SwrCtx);
		if (m_Audio.CodecCtx)
			avcodec_free_context(&m_Audio.CodecCtx);
	}

	bool AVPlayer::ProcessVideoPacket(AVPacket* packet)
	{
		if (avcodec_send_packet(m_Video.CodecCtx, packet) < 0)
		{
			Log::CoreError("An error occurred while sending video packet to decoder.");
			return false;
		}

		int result = avcodec_receive_frame(m_Video.CodecCtx, m_Frame);
		if (result == AVERROR(EAGAIN) || result == AVERROR_EOF)
		{
			Log::CoreError("Video should not raise EAGAIN or EOF");
			return false;
		}
		else if (result < 0)
		{
			Log::CoreError("An error occurred while receiving frame from decoder.");
			return false;
		}

		m_Video.NextPTS = GetPTS(m_Frame->pts, m_Frame->pkt_dts);
		LI_CORE_ASSERT(m_Frame->width == m_Video.Width, "Invalid width.");
		LI_CORE_ASSERT(m_Frame->height == m_Video.Height, "Invalid height.");

		// Convert colors.
		const int dst_stride = m_Video.Width * VideoData::NumOutChannels;
		// Flip video vertically.
		// http://ffmpeg-users.933282.n4.nabble.com/Flip-in-sws-scale-td939665.html
		for (int i = 0; i < m_Video.NumOutChannels; i++)
		{
			if (i) 
				m_Frame->data[i] += m_Frame->linesize[i] * ((m_Frame->height >> 1) - 1);
			else
				m_Frame->data[i] += m_Frame->linesize[i] * (m_Frame->height - 1);
			
			m_Frame->linesize[i] = -m_Frame->linesize[i];
		}

		sws_scale(m_Video.SwsContext, m_Frame->data, m_Frame->linesize, 0, m_Video.Height, &m_Video.PixelBuffer, &dst_stride);

		return true;
	}

	bool AVPlayer::ProcessAudioPacket(AVPacket* packet)
	{
		if (avcodec_send_packet(m_Audio.CodecCtx, packet) < 0)
		{
			Log::CoreError("An error occurred while sending audio packet to decoder.");
			return false;
		}

		int result = avcodec_receive_frame(m_Audio.CodecCtx, m_Frame);
		if (result == AVERROR(EAGAIN))
			return true;
		else if (result == AVERROR_EOF)
		{
			Log::CoreWarn("Video ended with EOF on audio packet");
			return false;
		}
		else if (result < 0)
		{
			Log::CoreError("An error occurred while receiving frame from decoder.");
			return false;
		}

		// Skip audio frames if behind.
		int64_t pts = GetPTS(m_Frame->pts, m_Frame->pkt_dts);
		if (m_Elapsed > Li::Duration::fsec(pts * m_Video.TimeBase + m_Frame->nb_samples / (float)m_Audio.Frequency))
		{
			m_Audio.DiscardCount++;
			return true;
		}
		else if (m_Audio.DiscardCount > 0)
		{
			Log::CoreWarn("Discarded {} audio packets", m_Audio.DiscardCount);
			m_Audio.DiscardCount = 0;
		}

		if (m_Audio.Resample)
		{
			// Init resampling destination data store.
			if (m_Audio.DstData == nullptr)
			{
				// Equal because sample rate must be identical.
				m_Audio.MaxNumSamples = m_Frame->nb_samples;

				result = av_samples_alloc_array_and_samples(&m_Audio.DstData, nullptr,
					m_Audio.Channels, m_Frame->nb_samples, m_Audio.OutFormat, 0);
				if (result < 0 || m_Audio.DstData == nullptr)
				{
					Log::CoreError("Failed to allocate samples for audio resampling");
					return true;
				}
			}

			// Equal because sample rate must be identical.
			int out_num_samples = static_cast<int>(swr_get_delay(m_Audio.SwrCtx, m_Audio.Frequency)) + m_Frame->nb_samples;
			// Resize output buffer if necessary.
			if (out_num_samples > m_Audio.MaxNumSamples)
			{
				av_freep(&m_Audio.DstData[0]);
				result = av_samples_alloc(m_Audio.DstData, nullptr, m_Audio.Channels, m_Frame->nb_samples, m_Audio.OutFormat, 1);
			}

			result = swr_convert(m_Audio.SwrCtx, m_Audio.DstData, out_num_samples, const_cast<const uint8_t**>(m_Frame->data), m_Frame->nb_samples);
			if (result < 0)
			{
				Log::CoreError("An error occurred while resampling audio.");
				return true;
			}

			if (m_Audio.Queue == nullptr)
				m_Audio.Queue = MakeUnique<AudioQueue>(m_Audio.OutBytesPerSample, m_Audio.Channels, m_Audio.Frequency, out_num_samples, m_Audio.Frequency);

			if (m_Audio.Started)
			{
				// Sync video pts to audio pts.
				if (m_Audio.Queue->SubmitAudio(m_Audio.DstData[0], out_num_samples))
					m_Video.NextPTS = pts;
			}
			// Check if queue starts the audio.
			else
				m_Audio.Queue->SubmitAudio(m_Audio.DstData[0], out_num_samples);
		}
		else
		{
			if (m_Audio.Queue == nullptr)
			{
				Log::CoreWarn("Non-resampled audio is not tested.");
				m_Audio.Queue = MakeUnique<AudioQueue>(m_Audio.OutBytesPerSample, m_Audio.Channels, m_Audio.Frequency, m_Frame->nb_samples, m_Audio.Frequency);
			}

			if (m_Audio.Started)
			{
				// Sync video pts to audio pts.
				if (m_Audio.Queue->SubmitAudio(m_Audio.DstData[0], m_Frame->nb_samples))
					m_Video.NextPTS = pts;
			}
			else
				m_Audio.Queue->SubmitAudio(m_Frame->data[0], m_Frame->nb_samples);
		}
		return true;
	}

	int64_t AVPlayer::GetPTS(int64_t frame_pts, int64_t dts)
	{
		if (dts != AV_NOPTS_VALUE)
		{
			if (dts <= m_Video.CodecCtx->pts_correction_last_dts)
				m_Video.CodecCtx->pts_correction_num_faulty_dts++;
			m_Video.CodecCtx->pts_correction_last_dts = dts;
		}
		else if (frame_pts != AV_NOPTS_VALUE)
		{
			m_Video.CodecCtx->pts_correction_last_dts = frame_pts;
		}

		if (frame_pts != AV_NOPTS_VALUE)
		{
			if (frame_pts <= m_Video.CodecCtx->pts_correction_last_pts)
				m_Video.CodecCtx->pts_correction_num_faulty_pts++;
			m_Video.CodecCtx->pts_correction_last_pts = frame_pts;
		}
		else if (dts != AV_NOPTS_VALUE)
		{
			m_Video.CodecCtx->pts_correction_last_pts = dts;
		}

		if (frame_pts != AV_NOPTS_VALUE &&
			(dts == AV_NOPTS_VALUE || m_Video.CodecCtx->pts_correction_num_faulty_pts <= m_Video.CodecCtx->pts_correction_num_faulty_dts))
			return frame_pts;
		return dts;
	}

	static float RationalToFloat(const AVRational& rational)
	{
		return (float)rational.num / (float)rational.den;
	}
}
