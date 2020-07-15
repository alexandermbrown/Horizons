#include "pch.h"
#include "ConfigVar.h"


ConfigVar::ConfigVar(const std::string& name, uint32_t flags, bool save)
	: m_Name(name), m_Flags(flags), m_Save(save)
{
}

ConfigVar::ConfigVar(const std::string& name, bool value, uint32_t flags, bool save)
	: m_Name(name), m_Flags(flags), m_Save(save)
{
	m_BoolValue = value;
}

ConfigVar::ConfigVar(const std::string& name, int value, uint32_t flags, bool save)
	: m_Name(name), m_Flags(flags), m_Save(save)
{
	m_IntValue = value;
}

ConfigVar::ConfigVar(const std::string& name, unsigned int value, uint32_t flags, bool save)
	: m_Name(name), m_Flags(flags), m_Save(save)
{
	m_UnsignedValue = value;
}

ConfigVar::ConfigVar(const std::string& name, float value, uint32_t flags, bool save)
	: m_Name(name), m_Flags(flags), m_Save(save)
{
	m_FloatValue = value;
}

bool ConfigVar::SetFromString(const std::string& value)
{
	if (m_Flags & HZ_CVAR_BOOL)
	{
		if (value == "true" || value == "on" || value == "t" || value == "1")
		{
			SetBool(true);
		}
		else if (value == "false" || value == "off" || value == "f" || value == "0")
		{
			SetBool(false);
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
			SetInt(std::stoi(value));
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
			// Try to stop the user from entering negative values.
			if (value[0] == '-')
				return false;

			SetUnsigned((unsigned int)std::stoul(value));
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
			SetFloat(std::stof(value));
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

std::string ConfigVar::GetString() const
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
