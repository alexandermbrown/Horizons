#pragma once

#include <cstdint>
#include <string>

namespace li
{
	class AudioBuffer
	{
	public:
		AudioBuffer();
		virtual ~AudioBuffer();

		void SetData(const void* pcm_data, uint32_t size, int channels, int bytes_per_sample, int frequency);

		inline uint32_t GetBufferID() const { return m_BufferID; }

		/**
		 * Creates an AudioBuffer from an ogg file.
		 * @param filename the path to the ogg file
		 * @returns an AudioBuffer reference, nullptr on failure
		 */
		static Ref<AudioBuffer> FromOggFile(const std::string& filename);

		/**
		 * Creates an AudioBuffer from an ogg file in memory.
		 * @param ogg_source pointer to the ogg file data in memory
		 * @param the size size of the ogg_source in memory, in bytes
		 * @returns an AudioBuffer reference, nullptr on failure
		 */
		static Ref<AudioBuffer> FromOggMemory(const void* ogg_source, uint32_t size);

	private:
		uint32_t m_BufferID;
	};
}