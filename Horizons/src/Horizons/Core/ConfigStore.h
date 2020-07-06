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

private:

	void LoadType(const char* str_type, uint32_t type, CSimpleIniA& ini);
	void SaveValues(std::unordered_map<std::string, ConfigVar>& store, CSimpleIniA& ini);

	void SetAllDefault();

	std::unordered_map<std::string, ConfigVar> m_ConfigVars;

	std::unordered_map<std::string, ConfigVar> m_Templates;
};
