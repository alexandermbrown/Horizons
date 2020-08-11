#include "lipch.h"
#include "LocaleLoader.h"

#define LI_READ_FILE(file, ptr, size, pos) (file).read(ptr, size); pos += (file).gcount()

namespace li
{
	Ref<Locale> LocaleLoader::Load(const Assets::Locale* locale)
	{
		Ref<Locale> localeref = CreateRef<Locale>(locale->name()->str());
		static_assert(sizeof(wchar_t) == sizeof(int16_t));

		for (const Assets::LocaleEntry* entry : *locale->entries())
		{
			std::wstring value = (wchar_t*)entry->value()->data();
			localeref->Set(entry->key()->str(), value);
		}

		return localeref;
	}
}