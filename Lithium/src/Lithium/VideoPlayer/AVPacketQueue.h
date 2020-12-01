#pragma once

#include "readerwriterqueue/readerwriterqueue.h"

#include <mutex>
#include <condition_variable>

struct AVFormatContext;
struct AVPacket;
struct AVStream;

namespace li
{
	class AVPacketQueue
	{
	public:
		AVPacketQueue(int min_count, int max_count);
		AVPacketQueue(const AVPacketQueue&) = delete;
		~AVPacketQueue();

		void Enqueue(AVPacket* packet);
		AVPacket* Dequeue();

		void Stop();

	private:
		int m_MinCount;
		int m_MaxCount;
		moodycamel::ReaderWriterQueue<AVPacket*> m_Packets;

		std::mutex m_Mutex;
		std::condition_variable m_CondVar;
		bool m_Ready;

		int m_NumDequeuedSinceMax;

		std::atomic<bool> m_Stop;
	};
}
