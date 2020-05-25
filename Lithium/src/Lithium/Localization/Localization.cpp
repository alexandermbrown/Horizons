#include "lipch.h"
#include "Localization.h"

namespace li
{
	Scope<Localization> Localization::s_Instance = CreateScope<Localization>();

	Locale::Locale(const std::string& code) : m_Code(code), m_Strings() {}

	Localization::Localization() : m_Locales() {}

	void Localization::SetLocaleImpl(const std::string& localeCode)
	{
		for (const Ref<Locale>& locale : m_Locales) {
			if (locale->GetCode() == localeCode) {
				m_Selected = locale;
				return;
			}
		}
		LI_CORE_ERROR("Failed to find locale '{}'.", localeCode);
	}

}

