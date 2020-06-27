#pragma once

#include "Horizons/Core/Core.h"
#include "Lithium.h"

#include <string>
#include <unordered_map>

typedef enum
{
	HZ_CVAR_NULL = BIT(0),
	HZ_CVAR_BOOL = BIT(1),
	HZ_CVAR_INT = BIT(2),
	HZ_CVAR_UNSIGNED = BIT(3),
	HZ_CVAR_FLOAT = BIT(4)
} ConfigVarFlags;


class ConfigVar
{
public:

	ConfigVar(const std::string& name, bool value, uint32_t flags, const std::string& description);
	ConfigVar(const std::string& name, int value, uint32_t flags, const std::string& description);
	ConfigVar(const std::string& name, unsigned int value, uint32_t flags, const std::string& description);
	ConfigVar(const std::string& name, float value, uint32_t flags, const std::string& description);

	bool SetFromString(const std::string& value);

	inline const std::string& GetName() const { return m_Name; }
	inline uint32_t GetFlags() const { return m_Flags; }
	inline const std::string& GetDescription() const { return m_Description; }

	std::string GetString();

	inline bool GetBool()
	{
		LI_ASSERT(m_Flags & HZ_CVAR_BOOL, "CVar is not of type HZ_CVAR_BOOL");
		return m_BoolValue;
	}

	inline int GetInt()
	{
		LI_ASSERT(m_Flags & HZ_CVAR_INT, "CVar is not of type HZ_CVAR_INT");
		return m_IntValue;
	}

	inline unsigned int GetUnsigned()
	{
		LI_ASSERT(m_Flags & HZ_CVAR_UNSIGNED, "CVar is not of type HZ_CVAR_UNSIGNED");
		return m_UnsignedValue;
	}

	inline float GetFloat()
	{
		LI_ASSERT(m_Flags & HZ_CVAR_FLOAT, "CVar is not of type HZ_CVAR_FLOAT");
		return m_FloatValue;
	}
	
	inline void Set(bool value)
	{
		LI_ASSERT(m_Flags & HZ_CVAR_BOOL, "Cannot set CVar of type {} to bool.");
		m_BoolValue = value;
	};

	inline void Set(int value)
	{
		LI_ASSERT(m_Flags & HZ_CVAR_INT, "Cannot set CVar of type {} to int.");
		m_IntValue = value;
	};

	inline void Set(unsigned int value)
	{
		LI_ASSERT(m_Flags & HZ_CVAR_UNSIGNED, "Cannot set CVar of type {} to unsigned int.");
		m_UnsignedValue = value;
	};

	inline void Set(float value)
	{
		LI_ASSERT(m_Flags & HZ_CVAR_FLOAT, "Cannot set CVar of type {} to float.");
		m_FloatValue = value;
	};

private:

	union
	{
		bool m_BoolValue;
		int m_IntValue;
		unsigned int m_UnsignedValue;
		float m_FloatValue;
	};

	std::string m_Name;
	std::string m_Description;

	uint32_t m_Flags;
};

class ConfigStore
{
public:

	ConfigStore();

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

	std::unordered_map<std::string, ConfigVar> m_ConfigVars;
};
