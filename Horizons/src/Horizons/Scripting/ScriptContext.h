#pragma once

#include "Lithium.h"
#include "sol.hpp"

class ScriptContext
{
public:
	void InitApp(const char* lua_entry_path);

	inline sol::state& GetLua() { return m_Lua; }

private:
	void LoadFunctions();

	sol::state m_Lua;
};
