#include "lipch.h"
#include "LocaleLoader.h"

#define LI_READ_FILE(file, ptr, size, pos) (file).read(ptr, size); pos += (file).gcount()

namespace li
{
	LocaleArgs* LocaleArgs::Deserialize(zstr::ifstream* inFile, size_t* pos)
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

		return new LocaleArgs(name, keysLength, keysData, valuesLength, valuesData);
	}

	LocaleArgs::~LocaleArgs()
	{
		delete[] m_KeysData;
		delete[] m_ValuesData;
	}

	Ref<Locale> LocaleArgs::Create()
	{
		Ref<Locale> locale = CreateRef<Locale>(m_Name);

		size_t keysOffset = 0;
		size_t valuesOffset = 0;

		while (keysOffset < m_KeysLength && valuesOffset < m_ValuesLength)
		{
			locale->Set(&m_KeysData[keysOffset], &m_ValuesData[valuesOffset]);

			keysOffset += strlen(&m_KeysData[keysOffset]) + 1ULL;
			valuesOffset += wcslen(&m_ValuesData[valuesOffset]) + 1ULL;
		}

		LI_CORE_ASSERT(keysOffset == m_KeysLength && valuesOffset == m_ValuesLength, "Invalid key/value pair data!");

		return locale;
	}
}