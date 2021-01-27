#include "pch.h"
#include "ScriptContext.h"

#include "Horizons.h"
#include "ScriptScene.h"
#include "ScriptLayer.h"

#include "Horizons/Scenes/NativeScenes.h"
#include "UIFunctions.h"

#include "entt/entt.hpp"

void ScriptContext::InitApp(const char* lua_entry_path)
{
	m_Lua.open_libraries(sol::lib::base, sol::lib::jit, sol::lib::package);
	
	m_Lua.create_named_table("App");

	LoadFunctions();

	try {
		sol::protected_function_result result = m_Lua.safe_script_file(lua_entry_path);
		if (!result.valid())
		{
			sol::error error = result;
			Li::Log::Error("{}", error.what());
		}
	}
	catch (...)
	{
		Li::Log::Error("Script Error!");
	}
}

void ScriptContext::LoadFunctions()
{
	m_Lua.new_enum<AppState>("AppState", {
		{ "None", AppState::None },
		{ "SplashScreen", AppState::SplashScreen },
		{ "MainMenu", AppState::MainMenu },
		{ "InGame", AppState::InGame },
	});

	sol::table app = m_Lua["App"];

	app.set_function("SetState", [](AppState app_state) {
		Li::Log::Trace("Setting app state to {}", app_state);
		Li::Application::Get<Horizons>().GetConfig().Set<int>("app_state", static_cast<int>(app_state));
	});

	app.set_function("SetDebugUIBlockEvents", [](bool block) {
		Li::Log::Trace("Setting debug ui block events to {}", block);
#ifndef LI_DIST
		Li::Application::Get<Horizons>().GetImGuiRenderer()->SetBlockEvents(block);
#endif
	});

	app.set_function("TransitionScene", [](std::string scene_name, bool instant) {
		Li::Unique<Li::Scene> scene = GetNativeScene(scene_name);
		if (scene)
			Li::Application::Get().Transition(std::move(scene), instant);
	});

	app.set_function("TransitionScriptScene", [](std::string scene_class_name, bool instant) {
		Li::Application::Get().Transition(Li::MakeUnique<ScriptScene>(scene_class_name), instant);
	});

	app.set_function("PushScriptLayer", [](std::string name) {
		Li::Application::Get().PushLayer(Li::MakeUnique<ScriptLayer>(name));
	});

	app.set_function("PopLayer", [](std::string name) {
		Li::Application::Get().PopLayer(name);
	});

	UIFunctions::LoadFunctions(m_Lua);
}
