#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Localization/Localization.h"

#include "zstr/zstr.hpp"
#include "ResourceArgs.h"

namespace li
{
	class LocaleArgs : public ResourceArgs
	{
	public:
		static LocaleArgs* Deserialize(zstr::ifstream* inFile, size_t* pos);

		LocaleArgs(const std::string& name, size_t keysLength, char* keysData, size_t valuesLength, wchar_t* valuesData)
			: ResourceArgs(SegmentType::Locale, name), m_KeysLength(keysLength), m_KeysData(keysData),
			m_ValuesLength(valuesLength), m_ValuesData(valuesData) {}

		virtual ~LocaleArgs();

		Ref<Locale> Create();

	private:
		size_t m_KeysLength;
		char* m_KeysData;

		size_t m_ValuesLength;
		wchar_t* m_ValuesData;
	};
}