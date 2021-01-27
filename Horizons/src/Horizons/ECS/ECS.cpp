#include "pch.h"
#include "ECS.h"

#include "Horizons.h"

void ECS::AddSystem(const std::string& name)
{
	const SystemRegistry& registry = Li::Application::Get<Horizons>().GetSystemRegistry();
	const System* system = registry.GetSystem(name);
	
	if (system)
	{
		if (system->OnInit)
			m_OnInitFns.push_back(system->OnInit);
		if (system->OnEvent)
			m_OnEventFns.push_back(system->OnEvent);
		if (system->OnUpdate)
			m_OnUpdateFns.push_back(system->OnUpdate);
		if (system->OnRender)
			m_OnRenderFns.push_back(system->OnRender);
		if (system->OnDeinit)
			m_OnDeinitFns.push_back(system->OnDeinit);
	}
	else
		Li::Log::Error("System {} does not exist!", name);
}

void ECS::OnInit()
{
	for (const System::OnInitFn& OnInitFn : m_OnInitFns)
		OnInitFn(m_Registry);
}

void ECS::OnEvent(SDL_Event* event)
{
	for (const System::OnEventFn& OnEventFn : m_OnEventFns)
		OnEventFn(m_Registry, event);
}

void ECS::OnUpdate(Li::Duration::us dt)
{
	for (const System::OnUpdateFn& OnUpdateFn : m_OnUpdateFns)
		OnUpdateFn(m_Registry, dt);
}

void ECS::OnRender()
{
	for (const System::OnRenderFn& OnRenderFn : m_OnRenderFns)
		OnRenderFn(m_Registry);
}

void ECS::OnDeinit()
{
	for (const System::OnDeinitFn& OnDeinitFn : m_OnDeinitFns)
		OnDeinitFn(m_Registry);
}
