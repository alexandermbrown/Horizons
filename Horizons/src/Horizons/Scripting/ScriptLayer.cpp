#include "pch.h"
#include "ScriptLayer.h"

#include "Horizons.h"

ScriptLayer::ScriptLayer(const std::string& lua_class_name)
	: Layer(lua_class_name)
{
	InitLuaClass(lua_class_name);
	InitSystems(lua_class_name);
	m_ECS.OnInit();
}

ScriptLayer::~ScriptLayer()
{
	m_ECS.OnDeinit();
}

void ScriptLayer::OnUpdate(Li::Duration::us dt)
{
	m_ECS.OnUpdate(dt);
	m_ECS.OnRender();
}

void ScriptLayer::OnEvent(SDL_Event* event)
{
	m_ECS.OnEvent(event);
}

#ifndef LI_DIST
void ScriptLayer::OnImGuiRender()
{
}
#endif

void ScriptLayer::InitLuaClass(const std::string& lua_class_name)
{
	sol::state& lua = Li::Application::Get<Horizons>().GetScriptContext().GetLua();

	sol::protected_function layer_ctor = lua["App"]["layers"][lua_class_name];
	if (layer_ctor.valid())
	{
		sol::protected_function_result result = layer_ctor(sol::light<entt::registry>(m_ECS.GetRegistry()));
		if (result.valid())
		{
			m_Layer = result;
			if (!m_Layer.valid())
			{
				sol::error error = result;
				Li::Log::Error("{}", error.what());
				LI_DEBUG_BREAK();
			}
		}
		else
		{
			sol::error error = result;
			Li::Log::Error("{}", error.what());
			LI_DEBUG_BREAK();
		}
	}
	else
	{
		Li::Log::Error("Invalid or non-existent Lua layer {}.", lua_class_name);
		LI_DEBUG_BREAK();
	}
}

void ScriptLayer::InitSystems(const std::string& lua_class_name)
{
	sol::table systems = m_Layer["systems"];
	if (systems.valid())
	{
		for (const auto& system_pair : systems)
		{
			if (system_pair.second.is<std::string>())
			{
				m_ECS.AddSystem(system_pair.second.as<std::string>());
			}
			else
			{
				Li::Log::Warn("({}) Expected string in system list but found {}",
					lua_class_name, sol::type_name(system_pair.second.lua_state(), system_pair.second.get_type()));
				LI_DEBUG_BREAK();
			}
		}
	}
	else Li::Log::Warn("Systems list not found in layer {}", lua_class_name);
}
