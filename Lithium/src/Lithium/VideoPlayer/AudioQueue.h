#pragma once

#include "Lithium/Audio/AudioBuffer.h"
#include <stdint.h>

namespace Li
{
	/**
	 * Audio queue, used for streaming audio.
	 */
	class AudioQueue
	{
	public:
		AudioQueue(int bytes_per_sample, int channels, int sample_rate, int frame_sample_count, int frequency);
		AudioQueue(const AudioQueue&) = delete;
		virtual ~AudioQueue();

		/**
		 * Adds audio to the end of the queue.
		 * @param data the raw PCM audio data
		 * @param num_samples the number of audio samples in data
		 * @returns true when inserting data causes audio to begin playback
		 */
		bool SubmitAudio(const uint8_t* data, int num_samples);

		void Play();
		void Stop();
		bool NeedsAudio() const;

		inline bool IsPlaying() const { return m_Playing; }

	private:
		int m_SamplesPerBuffer;
		int m_Channels;
		int m_BytesPerSample;
		int m_BufferSize;
		int m_Frequency;
		
		std::vector<Unique<AudioBuffer>> m_Buffers;
		uint32_t m_SourceID;

		int m_NumPartial;
		uint8_t *m_PartialData;
		int m_CurrentBuffer;
		
		bool m_Playing;

		constexpr static int NumBuffersTarget = 8;
	};
}
