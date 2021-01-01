#pragma once

#include "AVPacketQueue.h"

#include <string>
#include <thread>
#include <atomic>

struct AVFormatContext;
struct AVPacket;
struct AVStream;

namespace Li
{
	class AVReader
	{
	public:
		AVReader(const std::string& path);
		AVReader(const AVReader&) = delete;
		~AVReader();

		void Start();

		inline AVPacket* DequeueVideo() { return m_VideoPackets.Dequeue(); }
		inline AVPacket* DequeueAudio() { return m_AudioPackets.Dequeue(); }

		inline bool HasAudio() const { return m_AudioStreamIndex >= 0; }
		inline AVStream* GetVideoStream() { return m_VideoStream; }
		inline AVStream* GetAudioStream() { return m_AudioStream; }

		inline bool IsFinished() const { return m_Finished.load(); }

	private:
		void InitVideo(const std::string& path);
		void InitAudio(const std::string& path);
		void ReadThreadEntry();

		std::thread m_ReadThread;
		AVPacketQueue m_VideoPackets;
		AVPacketQueue m_AudioPackets;

		AVFormatContext* m_FormatCtx;
		AVPacket* m_Packet;

		AVStream* m_VideoStream;
		AVStream* m_AudioStream;

		int m_VideoStreamIndex;
		int m_AudioStreamIndex;

		std::atomic<bool> m_Running;
		std::atomic<bool> m_Finished;
	};
}
