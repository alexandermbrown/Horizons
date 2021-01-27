#pragma once

#include "yaml-cpp/yaml.h"

#include <string>
#include <fstream>
#include <tuple>
#include <unordered_map>
#include <map>
#include <variant>

template<typename... Types>
class ConfigStoreImpl
{
public:
	using Variant = std::variant<Types...>;
	using StoreMap = std::unordered_map<std::string, Variant>;

	template<typename ValueType>
	inline ValueType Get(const std::string& name) const
	{
		return std::get<ValueType>(m_Vars.at(name));
	}

	template<typename ValueType>
	inline const ValueType& GetRef(const std::string& name) const
	{
		return std::get<ValueType>(m_Vars.at(name));
	}

	template<typename ValueType>
	inline void Set(const std::string& name, ValueType value)
	{
		m_Vars[name] = value;
	}

	template<typename ValueType>
	inline void SetRef(const std::string& name, const ValueType& value)
	{
		m_Vars[name] = value;
	}

	inline void SetVariant(const std::string& name, const std::variant<Types...>& variant)
	{
		m_Vars[name] = variant;
	}

	inline typename StoreMap::iterator find(const std::string& name)
	{
		return m_Vars.find(name);
	}

	inline typename StoreMap::const_iterator find(const std::string& name) const
	{
		return m_Vars.find(name);
	}
	
	typename StoreMap::iterator begin() { return m_Vars.begin(); }
	typename StoreMap::iterator end() { return m_Vars.end(); }
	typename StoreMap::const_iterator begin() const { return m_Vars.begin(); }
	typename StoreMap::const_iterator end()	const { return m_Vars.end(); }

	struct DefaultVar
	{
		std::string Name;
		Variant Default;
	};

private:
	StoreMap m_Vars;
};

using ConfigStore = ConfigStoreImpl<bool, int, float, std::string>;
