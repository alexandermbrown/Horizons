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
		LocaleArgs(zstr::ifstream* inFile, size_t* pos);

		virtual ~LocaleArgs();

		Ref<Locale> Create();

	private:
		size_t m_KeysLength;
		char* m_KeysData;

		size_t m_ValuesLength;
		wchar_t* m_ValuesData;
	};
}