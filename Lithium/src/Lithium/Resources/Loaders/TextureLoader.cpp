#include "lipch.h"
#include "TextureLoader.h"

#define LI_READ_FILE(file, ptr, size, pos) (file).read(ptr, size); pos += (file).gcount()

namespace li
{
	Texture2DArgs* Texture2DArgs::Deserialize(zstr::ifstream* inFile, size_t* pos)
	{
		char name[64];
		li::FilterType min_filter;
		li::FilterType mag_filter;
		li::WrapType wrap_s;
		li::WrapType wrap_t;
		size_t imageSize;
		uint8_t* imageData;

		LI_READ_FILE(*inFile, (char*)&name, sizeof(name), *pos);
		LI_READ_FILE(*inFile, (char*)&min_filter, sizeof(min_filter), *pos);
		LI_READ_FILE(*inFile, (char*)&mag_filter, sizeof(mag_filter), *pos);
		LI_READ_FILE(*inFile, (char*)&wrap_s, sizeof(wrap_s), *pos);
		LI_READ_FILE(*inFile, (char*)&wrap_t, sizeof(wrap_t), *pos);
		LI_READ_FILE(*inFile, (char*)&imageSize, sizeof(imageSize), *pos);

		imageData = new uint8_t[imageSize];
		LI_READ_FILE(*inFile, (char*)imageData, imageSize, *pos);

		return new Texture2DArgs(name, imageSize, imageData, wrap_s, wrap_t, min_filter, mag_filter);
	}

	Texture2DArgs::~Texture2DArgs()
	{
		delete[] m_Data;
	}
}