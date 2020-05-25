#pragma once

#include "Lithium/Core/Core.h"

#include <unordered_map>
#include <string>

namespace li
{
	class Locale
	{
	public:

		Locale(const std::string& code);

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

		Localization();
		virtual ~Localization() = default;

		static inline void AddLocale(Ref<Locale> locale) 
		{
			s_Instance->AddLocaleImpl(locale);
		}

		static inline void SetLocale(const std::string& localeID)
		{
			s_Instance->SetLocaleImpl(localeID);
		}

		static inline const std::wstring& Get(const std::string& key)
		{ 
			return s_Instance->GetImpl(key);
		}

		static inline uint32_t GetLocaleCount()
		{
			return s_Instance->GetLocaleCountImpl();
		}

		static inline const std::string& GetCode()
		{
			return s_Instance->GetCodeImpl();
		}

	private:

		inline void AddLocaleImpl(Ref<Locale> locale)
		{ 
			m_Locales.push_back(locale);
		}

		void SetLocaleImpl(const std::string& localeCode);

		inline const std::wstring& GetImpl(const std::string& key) const
		{
			return m_Selected->Get(key);
		}

		inline uint32_t GetLocaleCountImpl() const
		{
			return (uint32_t)m_Locales.size();
		}

		inline const std::string& GetCodeImpl() const
		{
			return m_Selected->GetCode();
		}

		std::vector<Ref<Locale>> m_Locales;
		Ref<Locale> m_Selected;

		static Scope<Localization> s_Instance;
	};
}