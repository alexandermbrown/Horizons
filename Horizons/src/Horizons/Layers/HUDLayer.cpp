#include "pch.h"
#include "HUDLayer.h"

#include "Horizons/Rendering/RenderingComponents.h"
#include "Horizons/UI/UIComponents.h"
#include "Horizons/UI/UILayoutSystem.h"
#include "Horizons/UI/UIRenderSystem.h"
#include "Horizons/UI/UITransformSortSystem.h"
#include "Horizons/UI/UIHoverSystem.h"
#include "Horizons/UI/UIClickSystem.h"

#include "Horizons/Scripting/UIPrototypes.h"

HUDLayer::HUDLayer()
	: m_Registry()
{
	UILayoutSystem::Init(m_Registry);

	entt::entity context_ent = m_Registry.view<cp::ui_context>().front();
	UIPrototypes::InstantiateUIPrototype(m_Registry, context_ent, "hud");
	
	cp::ui_context& context = m_Registry.get<cp::ui_context>(context_ent);
	context.rebuild = true;
}

HUDLayer::~HUDLayer()
{
	UILayoutSystem::Shutdown(m_Registry);
}

void HUDLayer::OnAttach()
{

}

void HUDLayer::OnDetach()
{

}

void HUDLayer::OnUpdate(float dt)
{
	UILayoutSystem::Update(m_Registry);
	UITransformSortSystem::SortTransforms(m_Registry);

	li::Renderer::BeginUI();
	UIRenderSystem::Render(m_Registry);
	li::Renderer::EndUI();
	UIRenderSystem::RenderLabels(m_Registry);
}

void HUDLayer::OnImGuiRender()
{
	
}

void HUDLayer::OnEvent(SDL_Event* event)
{
	if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
	{
		UILayoutSystem::OnWindowResize(m_Registry, event->window.data1, event->window.data2);
	}
	else if (event->type == SDL_MOUSEMOTION)
	{
		UIHoverSystem::OnMouseMove(m_Registry, event->motion.x, event->motion.y);
	}
	else if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		UIClickSystem::OnMouseDown(m_Registry, event->button.x, event->button.y, event->button.button);
	}
	else if (event->type == SDL_MOUSEBUTTONUP)
	{
		UIClickSystem::OnMouseUp(m_Registry, event->button.x, event->button.y, event->button.button);
	}
}
