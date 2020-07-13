#include "pch.h"
#include "ConfigStore.h"

#include <filesystem>

ConfigStore::ConfigStore()
	: m_ConfigVars(), m_Templates()
{

}

void ConfigStore::LoadTemplate(const char* path)
{
	CSimpleIniA ini;
	ini.SetUnicode();
	
	SI_Error error = ini.LoadFile(path);
	LI_ASSERT(error >= 0, "Failed to load template ini source.");

	LoadType("bool", HZ_CVAR_BOOL, ini);
	LoadType("int", HZ_CVAR_INT, ini);
	LoadType("unsigned", HZ_CVAR_UNSIGNED, ini);
	LoadType("float", HZ_CVAR_FLOAT, ini);
}

void ConfigStore::LoadConfigFile(const wchar_t* path)
{
	CSimpleIniA ini;
	ini.SetUnicode();

	SI_Error error = ini.LoadFile(path);

	if (error == SI_FILE)
	{
		LI_WARN("Failed to open config file. Resetting config to default values.");
		LI_WARN("This is expected behavior on the first time running the program.");
		SetAllDefault();
	}
	else if (error < 0)
	{
		LI_WARN("Failed to load config file with unknown error. Resetting config to default values.");
		SetAllDefault();
	}

	for (const auto& [key, template_value] : m_Templates)
	{
		const char* user_value = ini.GetValue("config", key.c_str());

		ConfigVar cvar = ConfigVar(key, template_value.GetFlags(), true);
		if (user_value)
		{
			if (!cvar.SetFromString(user_value))
			{
				LI_WARN("Failed to set '{}' to '{}' in user config. Resetting to default value.", key, user_value);
				cvar = template_value;
			}
		}
		else
		{
			LI_WARN("Config variable {} not found in user config. Resetting to default value.", key, user_value);
			cvar = template_value;
		}
		m_ConfigVars.insert_or_assign(key, cvar);
	}
}

void ConfigStore::SaveConfigFile(const wchar_t* path)
{
	CSimpleIniA ini;
	ini.SetUnicode();

	SI_Error error = ini.LoadFile(path);

	if (error == SI_FILE)
	{
		LI_WARN("Writing new config file.");
		SaveValues(m_ConfigVars, ini);
	}
	else if (error < 0)
	{
		LI_WARN("Failed to load config file with unknown error. Resetting config to default values.");
	}
	else
	{
		SaveValues(m_ConfigVars, ini);
	}

	std::filesystem::path fs_path = path;
	std::filesystem::create_directories(fs_path.parent_path());

	error = ini.SaveFile(path);

	if (error < 0)
		LI_ERROR("Failed to write config file to path.");
	else
		LI_INFO("Saved config.ini file.");
}

void ConfigStore::LoadType(const char* str_type, uint32_t type, CSimpleIniA& ini)
{
	CSimpleIniA::TNamesDepend ini_keys;
	ini.GetAllKeys(str_type, ini_keys);

	for (const CSimpleIniA::Entry& key : ini_keys)
	{
		LI_ASSERT(key.pItem, "Key must not be empty.");

		const char* value_str = ini.GetValue(str_type, key.pItem);

		std::string key_name = key.pItem;
		ConfigVar cvar = ConfigVar(key_name, type, true);
		if (!cvar.SetFromString(value_str))
		{
			LI_CORE_ERROR("Failed to set cvar template of type {} to {}", str_type, value_str);
			cvar.SetFromString("0");
		}

		m_Templates.insert_or_assign(key_name, cvar);
	}
}

void ConfigStore::SaveValues(std::unordered_map<std::string, ConfigVar>& store, CSimpleIniA& ini)
{
	for (const auto& [key, value] : store)
	{
		if (value.DoSave())
		{
			SI_Error status = ini.SetValue("config", key.c_str(), value.GetString().c_str());
			LI_ASSERT(status >= 0, "Failed to set value {} to {} in ini file.", key, value.GetString());
		}
	}
}

void ConfigStore::SetAllDefault()
{
	m_ConfigVars = m_Templates;
}
