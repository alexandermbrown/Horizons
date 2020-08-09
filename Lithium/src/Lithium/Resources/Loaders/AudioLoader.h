#pragma once

#include "Lithium/Audio/Audio.h"
#include "zstr/zstr.hpp"

#include "ResourceArgs.h"

namespace li
{
	class AudioArgs : public ResourceArgs
	{
	public:
		AudioArgs(zstr::ifstream* inFile, size_t* pos);

		virtual ~AudioArgs();

		inline Ref<Audio> Create()
		{
			return CreateRef<Audio>(m_GlslData, m_Size);
		};

	private:
		uint8_t* m_GlslData;
		uint32_t m_Size;
	};
}