#pragma once

#include <cstdint>
#include <string>

namespace li
{
	class Audio
	{
	public:

		Audio(const std::string& filename);
		Audio(void* source, uint32_t length);
		virtual ~Audio();

		inline uint32_t GetBufferID() const { return m_BufferID; }

	private:

		uint32_t m_BufferID;
	};
}