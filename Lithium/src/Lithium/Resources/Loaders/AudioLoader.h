#pragma once

#include "Lithium/Audio/Audio.h"
#include "zstr/zstr.hpp"

#include "ResourceArgs.h"

namespace li
{
	class AudioArgs : public ResourceArgs
	{
	public:
		static AudioArgs* Deserialize(zstr::ifstream* inFile, size_t* pos);

		AudioArgs(const std::string& name, uint8_t* data, uint32_t size)
			: ResourceArgs(SegmentType::Audio, name), m_Data(data), m_Size(size) {}

		virtual ~AudioArgs();

		inline Ref<Audio> Create()
		{
			return CreateRef<Audio>(m_Data, m_Size);
		};

	private:
		uint8_t* m_Data;
		uint32_t m_Size;
	};
}