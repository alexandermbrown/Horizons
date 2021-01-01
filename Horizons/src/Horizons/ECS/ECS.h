#pragma once

#include "System.h"

#include "Lithium.h"
#include "entt/entt.hpp"
#include <vector>

/** Represents an Entity Component System */
class ECS
{
public:
	void AddSystem(const std::string& name);

	void OnInit();
	void OnEvent(SDL_Event* event);
	void OnUpdate(Li::Duration::us dt);
	void OnRender();
	void OnDeinit();

	inline entt::registry& GetRegistry() { return m_Registry; }

private:
	entt::registry m_Registry;

	std::vector<System::OnInitFn> m_OnInitFns;
	std::vector<System::OnEventFn> m_OnEventFns;
	std::vector<System::OnUpdateFn> m_OnUpdateFns;
	std::vector<System::OnRenderFn> m_OnRenderFns;
	std::vector<System::OnDeinitFn> m_OnDeinitFns;
};
