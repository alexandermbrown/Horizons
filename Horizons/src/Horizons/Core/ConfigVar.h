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
	ConfigVar(const std::string& name, uint32_t flags, bool save);

	ConfigVar(const std::string& name, bool value, uint32_t flags, bool save);
	ConfigVar(const std::string& name, int value, uint32_t flags, bool save);
	ConfigVar(const std::string& name, unsigned int value, uint32_t flags, bool save);
	ConfigVar(const std::string& name, float value, uint32_t flags, bool save);

	bool SetFromString(const std::string& value);

	inline const std::string& GetName() const { return m_Name; }
	inline uint32_t GetFlags() const { return m_Flags; }

	std::string GetString() const;

	inline bool DoSave() const { return m_Save; }
	inline bool IsDirty() const { return m_Dirty; }
	inline void Clean() { m_Dirty = false; }

	inline bool GetBool() const
	{
		LI_ASSERT(m_Flags & HZ_CVAR_BOOL, "CVar is not of type HZ_CVAR_BOOL");
		return m_BoolValue;
	}

	inline int GetInt() const
	{
		LI_ASSERT(m_Flags & HZ_CVAR_INT, "CVar is not of type HZ_CVAR_INT");
		return m_IntValue;
	}

	inline unsigned int GetUnsigned() const
	{
		LI_ASSERT(m_Flags & HZ_CVAR_UNSIGNED, "CVar is not of type HZ_CVAR_UNSIGNED");
		return m_UnsignedValue;
	}

	inline float GetFloat() const
	{
		LI_ASSERT(m_Flags & HZ_CVAR_FLOAT, "CVar is not of type HZ_CVAR_FLOAT");
		return m_FloatValue;
	}
	
	inline void SetBool(bool value)
	{
		LI_ASSERT(m_Flags & HZ_CVAR_BOOL, "Cannot set CVar to bool.");
		if (m_BoolValue != value)
			m_Dirty = true;

		m_BoolValue = value;
	};

	inline void SetInt(int value)
	{
		LI_ASSERT(m_Flags & HZ_CVAR_INT, "Cannot set CVar to int.");
		if (m_IntValue != value)
			m_Dirty = true;

		m_IntValue = value;
	};

	inline void SetUnsigned(unsigned int value)
	{
		LI_ASSERT(m_Flags & HZ_CVAR_UNSIGNED, "Cannot set CVar to unsigned int.");
		if (m_UnsignedValue != value)
			m_Dirty = true;

		m_UnsignedValue = value;
	};

	inline void SetFloat(float value)
	{
		LI_ASSERT(m_Flags & HZ_CVAR_FLOAT, "Cannot set CVar to float.");
		if (m_FloatValue != value)
			m_Dirty = true;

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

	uint32_t m_Flags;

	bool m_Dirty = true;
	bool m_Save = false;
};
