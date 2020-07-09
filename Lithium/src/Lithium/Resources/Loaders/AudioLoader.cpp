#include "lipch.h"
#include "AudioLoader.h"

#define LI_READ_FILE(file, ptr, size, pos) (file).read(ptr, size); pos += (file).gcount()

namespace li
{
	AudioArgs* AudioArgs::Deserialize(zstr::ifstream* inFile, size_t* pos)
	{
		char name[64];
		size_t fileSize;

		LI_READ_FILE(*inFile, (char*)&name, sizeof(name), *pos);
		LI_READ_FILE(*inFile, (char*)&fileSize, sizeof(fileSize), *pos);

		uint8_t* fileData = new uint8_t[fileSize];
		LI_READ_FILE(*inFile, (char*)fileData, fileSize, *pos);

		return new AudioArgs(name, fileData, (uint32_t)fileSize);
	}

	AudioArgs::~AudioArgs()
	{
		delete[] m_Data;
	}
}