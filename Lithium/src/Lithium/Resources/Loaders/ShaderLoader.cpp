#include "lipch.h"
#include "ShaderLoader.h"

#define LI_READ_FILE(file, ptr, size, pos) (file).read(ptr, size); pos += (file).gcount()

namespace li
{
	ShaderArgs* ShaderArgs::Deserialize(zstr::ifstream* inFile, size_t* pos)
	{
		char name[64];
		size_t shaderSize;

		LI_READ_FILE(*inFile, (char*)&name, sizeof(name), *pos);
		LI_READ_FILE(*inFile, (char*)&shaderSize, sizeof(shaderSize), *pos);

		char* shaderData = new char[shaderSize + 1];
		LI_READ_FILE(*inFile, (char*)shaderData, shaderSize, *pos);
		shaderData[shaderSize] = '\0';

		return new ShaderArgs(name, shaderData);
	}

	ShaderArgs::~ShaderArgs()
	{
		delete[] m_Data;
	}
}