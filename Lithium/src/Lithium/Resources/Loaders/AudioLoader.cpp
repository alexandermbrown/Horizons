#include "lipch.h"
#include "AudioLoader.h"

#define LI_READ_FILE(file, ptr, size, pos) (file).read(ptr, size); pos += (file).gcount()

namespace li
{
	AudioArgs::AudioArgs(zstr::ifstream* inFile, size_t* pos)
		: ResourceArgs(SegmentType::Audio)
	{
		char name[64];

		LI_READ_FILE(*inFile, (char*)&name, sizeof(name), *pos);
		LI_READ_FILE(*inFile, (char*)&m_Size, sizeof(m_Size), *pos);

		m_GlslData = new uint8_t[m_Size];
		LI_READ_FILE(*inFile, (char*)m_GlslData, m_Size, *pos);

		m_Name = name;
	}

	AudioArgs::~AudioArgs()
	{
		delete[] m_GlslData;
	}
}