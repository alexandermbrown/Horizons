#pragma once

#include "System.h"

#include <unordered_map>
#include <string>
#include <utility>

class SystemRegistry
{
public:
	inline System& Register(const std::string& name)
	{
		return m_Systems[name];
	}

	inline const System* GetSystem(const std::string& name) const
	{
		auto it = m_Systems.find(name);
		if (it != m_Systems.end())
			return &it->second;
		return nullptr;
	}

private:
	std::unordered_map<std::string, System> m_Systems;
};
