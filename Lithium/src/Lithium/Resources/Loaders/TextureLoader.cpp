#include "lipch.h"
#include "TextureLoader.h"

#define LI_READ_FILE(file, ptr, size, pos) (file).read(ptr, size); pos += (file).gcount()

namespace li
{
	Texture2DArgs::Texture2DArgs(zstr::ifstream* inFile, size_t* pos)
		: ResourceArgs(SegmentType::Texture2D)
	{
		char name[64];

		LI_READ_FILE(*inFile, (char*)&name, sizeof(name), *pos);
		LI_READ_FILE(*inFile, (char*)&m_MinFilter, sizeof(m_MinFilter), *pos);
		LI_READ_FILE(*inFile, (char*)&m_MagFilter, sizeof(m_MagFilter), *pos);
		LI_READ_FILE(*inFile, (char*)&m_WrapS, sizeof(m_WrapS), *pos);
		LI_READ_FILE(*inFile, (char*)&m_WrapT, sizeof(m_WrapT), *pos);
		LI_READ_FILE(*inFile, (char*)&m_Size, sizeof(m_Size), *pos);

		m_GlslData = new uint8_t[m_Size];
		LI_READ_FILE(*inFile, (char*)m_GlslData, m_Size, *pos);

		m_Name = name;
	}

	Texture2DArgs::~Texture2DArgs()
	{
		delete[] m_GlslData;
	}
}