#pragma once

#include "ConfigVar.h"

#include "simpleini/simpleini.h"

class ConfigStore
{
public:
	ConfigStore();

	void LoadTemplate(const char* path);

	void LoadConfigFile(const wchar_t* path);
	void SaveConfigFile(const wchar_t* path);

	inline bool Contains(const std::string& name)
	{
		return m_ConfigVars.find(name) != m_ConfigVars.end();
	}

	inline void Add(const ConfigVar& var)
	{
		m_ConfigVars.insert({ var.GetName(), var });
	}

	inline ConfigVar& Get(const std::string& name)
	{
		LI_ASSERT(Contains(name), "Config variable '{}' not found.", name);
		return m_ConfigVars.at(name);
	}

	using ConfigMap = std::unordered_map<std::string, ConfigVar>;

	ConfigMap::iterator begin() { return m_ConfigVars.begin(); }
	ConfigMap::iterator end() { return m_ConfigVars.end(); }
	ConfigMap::const_iterator begin() const { return m_ConfigVars.begin(); }
	ConfigMap::const_iterator end()	const { return m_ConfigVars.end(); }

private:
	void LoadType(const char* str_type, uint32_t type, CSimpleIniA& ini);
	void SaveValues(std::unordered_map<std::string, ConfigVar>& store, CSimpleIniA& ini);
	void SetAllDefault();

	ConfigMap m_ConfigVars;
	ConfigMap m_Templates;
};
