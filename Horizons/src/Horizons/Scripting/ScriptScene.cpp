#include "pch.h"
#include "ScriptScene.h"

#include "Horizons.h"

void CallOptionalMethod(const sol::table& table, const char* fn_name);

ScriptScene::ScriptScene(const std::string& lua_class_name)
	: m_ClassName(lua_class_name)
{
	sol::state& lua = Li::Application::Get<Horizons>().GetScriptContext().GetLua();
	sol::protected_function scene_ctor = lua["App"]["scenes"][lua_class_name];
	if (scene_ctor.valid())
	{
		sol::protected_function_result result = scene_ctor();
		if (result.valid())
		{
			m_Scene = result;
			if (m_Scene.valid())
				lua["App"]["current_scene"] = m_Scene;
			else
			{
				lua["App"]["current_scene"] = sol::lua_nil;
				sol::error error = result;
				Li::Log::Error("{}", error.what());
				LI_DEBUG_BREAK();
			}
		}
		else
		{
			lua["App"]["current_scene"] = sol::lua_nil;
			sol::error error = result;
			Li::Log::Error("{}", error.what());
			LI_DEBUG_BREAK();
		}
	}
	else
	{
		lua["App"]["current_scene"] = sol::lua_nil;
		Li::Log::Error("Invalid or non-existent Lua scene {}.", lua_class_name);
		LI_DEBUG_BREAK();
	}
}

ScriptScene::~ScriptScene()
{
	sol::optional<sol::protected_function> transition_out = m_Scene["Deinit"];
	if (transition_out)
	{
		sol::protected_function_result result = transition_out.value()();
		if (!result.valid())
		{
			sol::error error = result;
			Li::Log::Error("{}", error.what());
			LI_DEBUG_BREAK();
		}
	}
}

void ScriptScene::OnShow()
{
	CallOptionalMethod(m_Scene, "OnShow");
}

void ScriptScene::OnTransition()
{
	CallOptionalMethod(m_Scene, "OnTransition");
}

void CallOptionalMethod(const sol::table& table, const char* fn_name)
{
	sol::optional<sol::protected_function> transition_out = table[fn_name];
	if (transition_out)
	{
		auto result = transition_out.value()();
		if (!result.valid())
		{
			sol::error error = result;
			Li::Log::Error("{}", error.what());
			LI_DEBUG_BREAK();
		}
	}
}
