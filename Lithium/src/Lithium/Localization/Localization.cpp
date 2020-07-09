#include "lipch.h"
#include "Localization.h"

namespace li
{
	Scope<Localization::LocalizationData> Localization::s_Data;

	Locale::Locale(const std::string& code) : m_Code(code), m_Strings() {}

	void Localization::Init()
	{
		s_Data = CreateScope<Localization::LocalizationData>();
		s_Data->m_Locales = std::vector<Ref<Locale>>();
	}

	void Localization::SetLocale(const std::string& localeCode)
	{
		for (const Ref<Locale>& locale : s_Data->m_Locales) {
			if (locale->GetCode() == localeCode) {
				s_Data->m_Selected = locale;
				return;
			}
		}
		LI_CORE_ERROR("Failed to find locale '{}'.", localeCode);
	}

}

