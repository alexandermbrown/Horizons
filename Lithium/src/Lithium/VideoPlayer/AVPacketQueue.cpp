#include "lipch.h"
#include "AVPacketQueue.h"

#include "Lithium/Core/Assert.h"
extern "C"
{
#include "libavformat/avformat.h"
}

namespace Li
{
	AVPacketQueue::AVPacketQueue(int min_count, int max_count)
		: m_MinCount(min_count), m_MaxCount(max_count), m_Packets(max_count),
		m_Ready(true), m_NumDequeuedSinceMax(0), m_Stop(false)
	{
	}

	AVPacketQueue::~AVPacketQueue()
	{
		AVPacket* packet;
		while (m_Packets.try_dequeue(packet))
			av_packet_unref(packet);
	}

	void AVPacketQueue::Enqueue(AVPacket* packet)
	{
#ifdef LI_ENABLE_ASSERTS
		int tries = 0;
#endif
		while (!m_Packets.try_enqueue(packet) && !m_Stop.load())
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_Ready = false;
			m_CondVar.wait(lock, [this] { return m_Ready; });

#ifdef LI_ENABLE_ASSERTS
			tries++;
			LI_CORE_ASSERT(tries < 2, "Conditional wait failed.");
#endif
		}
	}

	AVPacket* AVPacketQueue::Dequeue()
	{
		AVPacket* packet = nullptr;
		if (m_Packets.try_dequeue(packet))
		{
			LI_CORE_ASSERT(packet, "Invalid packet queued.");

			std::unique_lock<std::mutex> lock(m_Mutex);
			if (m_Ready)
			{
				lock.unlock();
				m_NumDequeuedSinceMax = 0;
			}
			else
			{
				if (m_MaxCount - m_NumDequeuedSinceMax <= m_MinCount)
				{
					m_Ready = true;
					m_NumDequeuedSinceMax = 0;
					lock.unlock();
					m_CondVar.notify_one();
				}
				else
				{
					m_NumDequeuedSinceMax++;
				}
			}
		}
		return packet;
	}

	void AVPacketQueue::Stop()
	{
		m_Stop = true;
		std::unique_lock<std::mutex> lock(m_Mutex);
		if (!m_Ready)
		{
			m_Ready = true;
			lock.unlock();
			m_CondVar.notify_one();
		}
	}
}