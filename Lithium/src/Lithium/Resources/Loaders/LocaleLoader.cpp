#include "lipch.h"
#include "LocaleLoader.h"

namespace Li::Loaders
{
	Ref<Locale> LoadLocale(const Assets::Locale* locale)
	{
		Ref<Locale> locale_ref = MakeRef<Locale>(locale->name()->str());
		static_assert(sizeof(wchar_t) == sizeof(int16_t));

		for (const Assets::LocaleEntry* entry : *locale->entries())
		{
			std::wstring value = (wchar_t*)entry->value()->data();
			locale_ref->Set(entry->key()->str(), value);
		}

		return locale_ref;
	}
}
