#include "lipch.h"
#include "ShaderLoader.h"

#define LI_READ_FILE(file, ptr, size, pos) (file).read(ptr, size); pos += (file).gcount()

namespace li
{
	ShaderArgs::ShaderArgs(zstr::ifstream* inFile, size_t* pos)
		: ResourceArgs(SegmentType::Shader)
	{
		char name[64];
		size_t glslSize;

		LI_READ_FILE(*inFile, (char*)&name, sizeof(name), *pos);

		LI_READ_FILE(*inFile, (char*)&glslSize, sizeof(glslSize), *pos);
		m_GlslData = new char[glslSize + 1];
		LI_READ_FILE(*inFile, (char*)m_GlslData, glslSize, *pos);
		m_GlslData[glslSize] = '\0';

		LI_READ_FILE(*inFile, (char*)&m_HlslVsSize, sizeof(m_HlslVsSize), *pos);
		m_HlslVsData = new char[m_HlslVsSize];
		LI_READ_FILE(*inFile, (char*)m_HlslVsData, m_HlslVsSize, *pos);

		LI_READ_FILE(*inFile, (char*)&m_HlslPsSize, sizeof(m_HlslPsSize), *pos);
		m_HlslPsData = new char[m_HlslPsSize];
		LI_READ_FILE(*inFile, (char*)m_HlslPsData, m_HlslPsSize, *pos);

		m_Name = name;
	}

	ShaderArgs::~ShaderArgs()
	{
		delete[] m_GlslData;
		delete[] m_HlslPsData;
	}
}