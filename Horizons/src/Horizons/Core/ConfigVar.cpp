#include "pch.h"
#include "ConfigVar.h"


ConfigVar::ConfigVar(const std::string& name, bool value, uint32_t flags, const std::string& description)
	: m_Name(name), m_Flags(flags), m_Description(description)
{
	m_BoolValue = value;
}

ConfigVar::ConfigVar(const std::string& name, int value, uint32_t flags, const std::string& description)
	: m_Name(name), m_Flags(flags), m_Description(description)
{
	m_IntValue = value;
}

ConfigVar::ConfigVar(const std::string& name, unsigned int value, uint32_t flags, const std::string& description)
	: m_Name(name), m_Flags(flags), m_Description(description)
{
	m_UnsignedValue = value;
}

ConfigVar::ConfigVar(const std::string& name, float value, uint32_t flags, const std::string& description)
	: m_Name(name), m_Flags(flags), m_Description(description)
{
	m_FloatValue = value;
}

bool ConfigVar::SetFromString(const std::string& value)
{
	if (m_Flags & HZ_CVAR_BOOL)
	{
		if (value == "true" || value == "on" || value == "t" || value == "1")
		{
			m_BoolValue = true;
		}
		else if (value == "false" || value == "off" || value == "f" || value == "0")
		{
			m_BoolValue = false;
		}
		else
		{
			return false;
		}
	}
	else if (m_Flags & HZ_CVAR_INT)
	{
		try
		{
			m_IntValue = std::stoi(value);
		}
		catch (const std::invalid_argument&)
		{
			return false;
		}
		catch (const std::out_of_range&)
		{
			return false;
		}
	}
	else if (m_Flags & HZ_CVAR_UNSIGNED)
	{
		try
		{
			m_UnsignedValue = std::stoul(value);
		}
		catch (const std::invalid_argument&)
		{
			return false;
		}
		catch (const std::out_of_range&)
		{
			return false;
		}
	}
	else if (m_Flags & HZ_CVAR_FLOAT)
	{
		try
		{
			m_FloatValue = std::stof(value);
		}
		catch (const std::invalid_argument&)
		{
			return false;
		}
		catch (const std::out_of_range&)
		{
			return false;
		}
	}
	return true;
}

std::string ConfigVar::GetString()
{
	if (m_Flags & HZ_CVAR_BOOL)
	{
		if (m_BoolValue) return std::string("true");
		else return std::string("false");
	}
	else if (m_Flags & HZ_CVAR_INT)
	{
		return std::to_string(m_IntValue);
	}
	else if (m_Flags & HZ_CVAR_UNSIGNED)
	{
		return std::to_string(m_UnsignedValue);
	}
	else if (m_Flags & HZ_CVAR_FLOAT)
	{
		return std::to_string(m_FloatValue);
	}
	return "UNKNOWN";
}

ConfigStore::ConfigStore()
	: m_ConfigVars()
{
}
