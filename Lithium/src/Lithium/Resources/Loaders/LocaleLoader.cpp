#include "lipch.h"
#include "LocaleLoader.h"

#define LI_READ_FILE(file, ptr, size, pos) (file).read(ptr, size); pos += (file).gcount()

namespace li
{
	LocaleArgs::LocaleArgs(zstr::ifstream* inFile, size_t* pos)
		: ResourceArgs(SegmentType::Locale)
	{
		char name[64];

		LI_READ_FILE(*inFile, (char*)&name, sizeof(name), *pos);

		LI_READ_FILE(*inFile, (char*)&m_KeysLength, sizeof(m_KeysLength), *pos);
		m_KeysData = new char[m_KeysLength];
		LI_READ_FILE(*inFile, m_KeysData, m_KeysLength, *pos);

		LI_READ_FILE(*inFile, (char*)&m_ValuesLength, sizeof(m_ValuesLength), *pos);
		m_ValuesData = new wchar_t[m_ValuesLength];
		LI_READ_FILE(*inFile, (char*)m_ValuesData, m_ValuesLength * sizeof(wchar_t), *pos);

		m_Name = name;
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