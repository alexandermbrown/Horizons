#include "lipch.h"
#include "LocaleLoader.h"

#define LI_READ_FILE(file, ptr, size, pos) (file).read(ptr, size); pos += (file).gcount()

namespace li
{
	Ref<Locale> LoadLocale(std::string* outname, zstr::ifstream* inFile, size_t* pos)
	{
		char name[64];

		size_t keysLength;
		size_t valuesLength;

		LI_READ_FILE(*inFile, (char*)&name, sizeof(name), *pos);

		LI_READ_FILE(*inFile, (char*)&keysLength, sizeof(keysLength), *pos);
		char* keysData = new char[keysLength];
		LI_READ_FILE(*inFile, keysData, keysLength, *pos);

		LI_READ_FILE(*inFile, (char*)&valuesLength, sizeof(valuesLength), *pos);
		wchar_t* valuesData = new wchar_t[valuesLength];
		LI_READ_FILE(*inFile, (char*)valuesData, valuesLength * sizeof(wchar_t), *pos);

		Ref<Locale> locale = CreateRef<Locale>(name);

		size_t keysOffset = 0;
		size_t valuesOffset = 0;

		while (keysOffset < keysLength && valuesOffset < valuesLength)
		{
			locale->Set(&keysData[keysOffset], &valuesData[valuesOffset]);
			
			keysOffset += strlen(&keysData[keysOffset]) + 1ULL;
			valuesOffset += wcslen(&valuesData[valuesOffset]) + 1ULL;
		}

		LI_CORE_ASSERT(keysOffset == keysLength && valuesOffset == valuesLength, "Invalid key/value pair data!");

		delete[] keysData;
		delete[] valuesData;

		*outname = std::string(name);
		return locale;
	}
}