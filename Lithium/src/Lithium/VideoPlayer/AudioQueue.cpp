#include "lipch.h"
#include "AudioQueue.h"

#include "Lithium/Audio/AudioCore.h"

#define AL_LIBTYPE_STATIC
#include "AL/al.h"

#include <fstream>

namespace Li
{
	AudioQueue::AudioQueue(int bytes_per_sample, int channels, int sample_rate, int frame_sample_count, int frequency)
		: m_BytesPerSample(bytes_per_sample), m_Channels(channels), m_NumPartial(0), m_CurrentBuffer(0),
			m_SamplesPerBuffer(frame_sample_count), m_Frequency(frequency), m_Playing(false)
	{
		m_BufferSize = m_SamplesPerBuffer * m_Channels * m_BytesPerSample;
		m_PartialData = new uint8_t[m_BufferSize];

		// TODO: make parameters.
		ALCall(alGenSources(1, &m_SourceID));
		ALCall(alSourcef(m_SourceID, AL_GAIN, 1.0f));
		ALCall(alSourcef(m_SourceID, AL_PITCH, 1.0f));
		// TODO: Follow camera. Maybe make video player only produce stereo?
		ALCall(alSource3f(m_SourceID, AL_POSITION, 0.0f, 0.0f, 0.0f));
		ALCall(alSource3f(m_SourceID, AL_VELOCITY, 0.0f, 0.0f, 0.0f));
		ALCall(alSourcei(m_SourceID, AL_LOOPING, AL_FALSE));
	}

	AudioQueue::~AudioQueue()
	{
		ALCall( alDeleteSources(1, &m_SourceID) );
		delete m_PartialData;
	}

	bool AudioQueue::SubmitAudio(const uint8_t* data, int num_samples)
	{
		LI_CORE_ASSERT(m_NumPartial < m_SamplesPerBuffer, "Partial sample count too large.");
		bool play = false;
		int pending_samples = m_NumPartial + num_samples;

		if (pending_samples >= m_SamplesPerBuffer)
		{
			int num_processed;
			ALCall( alGetSourcei(m_SourceID, AL_BUFFERS_PROCESSED, &num_processed) );

			if (num_processed == m_Buffers.size() && m_Playing)
			{
				Log::CoreWarn("Audio buffer ran out of audio!");
				play = m_Playing;
			}

			LI_CORE_ASSERT(num_processed >= 0 && num_processed <= m_Buffers.size(), "Invalid num processed.");
			
			while (pending_samples >= m_SamplesPerBuffer)
			{
				LI_CORE_ASSERT(pending_samples >= m_SamplesPerBuffer, "Invalid pending logic.");

				uint32_t buffer_id;
				if (num_processed == 0)
				{
					// insert new OpenAL buffer into vector.
					m_Buffers.insert(m_Buffers.begin() + m_CurrentBuffer, MakeUnique<AudioBuffer>());
					buffer_id = m_Buffers[m_CurrentBuffer]->GetBufferID();

					if (m_Buffers.size() > NumBuffersTarget)
						Log::CoreWarn("Resizing audio queue to {} buffers", m_Buffers.size());
				}
				else
				{
					buffer_id = m_Buffers[m_CurrentBuffer]->GetBufferID();
					ALCall( alSourceUnqueueBuffers(m_SourceID, 1, &buffer_id) );
					num_processed--;
				}

				if (m_NumPartial > 0)
				{
					size_t offset = (size_t)m_NumPartial * m_Channels * m_BytesPerSample;
					size_t data_copy_size = m_BufferSize - offset;
					memcpy(m_PartialData + offset, data, data_copy_size);
					m_Buffers[m_CurrentBuffer]->SetData(m_PartialData, m_BufferSize, m_Channels, m_BytesPerSample, m_Frequency);

					m_NumPartial = 0;
					data += data_copy_size;
				}
				else
				{
					m_Buffers[m_CurrentBuffer]->SetData(data, m_BufferSize, m_Channels, m_BytesPerSample, m_Frequency);
					data += m_BufferSize;
				}
				
				ALCall( alSourceQueueBuffers(m_SourceID, 1, &buffer_id) );

				m_CurrentBuffer = (m_CurrentBuffer + 1) % (int)m_Buffers.size();
				pending_samples -= m_SamplesPerBuffer;
			}

			// save leftover pending data.
			if (pending_samples > 0)
			{
				memcpy(m_PartialData, data, (size_t)pending_samples * m_Channels * m_BytesPerSample);
				m_NumPartial = pending_samples;
			}
		}
		else if (num_samples > 0)
		{
			memcpy(m_PartialData + ((size_t)m_NumPartial * m_Channels * m_BytesPerSample), data, (size_t)num_samples * m_Channels * m_BytesPerSample);
			m_NumPartial = pending_samples;
		}

		if (play)
			ALCall(alSourcePlay(m_SourceID));
		return play;
	}

	void AudioQueue::Play()
	{
		ALCall(alSourcePlay(m_SourceID));
		m_Playing = true;
	}

	void AudioQueue::Stop()
	{
		ALCall(alSourceStop(m_SourceID));
		m_Playing = false;
	}

	bool AudioQueue::NeedsAudio() const
	{
		if (m_Buffers.size() < NumBuffersTarget)
			return true;

		int num_processed;
		ALCall(alGetSourcei(m_SourceID, AL_BUFFERS_PROCESSED, &num_processed));
		return num_processed > 1;
	}
}
