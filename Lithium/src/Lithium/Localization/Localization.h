#pragma once

#include "Lithium/Core/Memory.h"

#include <unordered_map>
#include <string>

namespace Li
{
	class Locale
	{
	public:
		Locale(const std::string& code);
		virtual ~Locale() = default;

		inline const std::wstring& Get(const std::string& key) const
		{
#ifdef LI_ENABLE_ASSERTS
			LI_CORE_ASSERT(m_Strings.count(key) != 0, "Failed to find key '{}' in locale '{}'", key, m_Code);
#endif
			return m_Strings.at(key);
		}
		inline const std::string& GetCode() const { return m_Code; }

		inline const void Set(const std::string& key, const std::wstring& value)
		{
#ifdef LI_ENABLE_ASSERTS
			LI_CORE_ASSERT(m_Strings.count(key) == 0, "Key '{}' already exists in locale '{}'", key, m_Code);
#endif
			m_Strings[key] = value;
		}

	private:
		std::string m_Code;
		std::unordered_map<std::string, std::wstring> m_Strings;
	};

	class Localization
	{
	public:
		static void Init();

		static inline void AddLocale(Ref<Locale> locale)
		{ 
			s_Data->m_Locales.push_back(locale);
		}

		static void SetLocale(const std::string& localeCode);

		static inline const std::wstring& Get(const std::string& key)
		{
			return s_Data->m_Selected->Get(key);
		}

		static inline uint32_t GetLocaleCount()
		{
			return (uint32_t)s_Data->m_Locales.size();
		}

		static inline const std::string& GetCode()
		{
			return s_Data->m_Selected->GetCode();
		}

	private:
		struct LocalizationData
		{
			std::vector<Ref<Locale>> m_Locales;
			Ref<Locale> m_Selected;
		};

		static Unique<LocalizationData> s_Data;
	};
}
