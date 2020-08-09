#include "pch.h"
#include "HUDLayer.h"

#include "Horizons/Rendering/RenderingComponents.h"
#include "Horizons/UI/UIComponents.h"
#include "Horizons/UI/UISystem.h"
#include "Horizons/UI/UIRenderSystem.h"

HUDLayer::HUDLayer()
	: m_Registry()
{
	UISystem::Init(m_Registry);

	entt::entity context_ent = m_Registry.view<cp::ui_context>().front();

	entt::entity item_bar = m_Registry.create();

	auto& bar_element = m_Registry.emplace<cp::ui_element>(item_bar);
	bar_element.layout_contain = LAY_ROW;
	bar_element.layout_behave = LAY_BOTTOM | LAY_HFILL;
	bar_element.margins = lay_vec4_xyzw(30, 0, 30, 20);
	bar_element.height = 70;

	m_Registry.emplace<cp::ui_transform>(item_bar, 0.201f);
	m_Registry.emplace<cp::color>(item_bar, glm::vec4(0.4f, 0.4f, 0.4f, 0.6f));

	UISystem::AddChild(m_Registry, context_ent, item_bar);

	for (int i = 0; i < 10; i++)
	{
		entt::entity item_slot = m_Registry.create();

		auto& item_element = m_Registry.emplace<cp::ui_element>(item_slot);
		item_element.layout_behave = LAY_CENTER;
		item_element.width = 50;
		item_element.height = 50;
		item_element.margins = lay_vec4_xyzw(10, 0, 10, 0);

		m_Registry.emplace<cp::ui_transform>(item_slot, 0.202f);
		m_Registry.emplace<cp::color>(item_slot, glm::vec4(0.7f, 0.5f, 0.4f, 0.7f));

		UISystem::AddChild(m_Registry, item_bar, item_slot);
	}

	cp::ui_context& context = m_Registry.get<cp::ui_context>(context_ent);
	context.rebuild = true;
	context.recalculate = true;
}

HUDLayer::~HUDLayer()
{
	UISystem::Shutdown(m_Registry);
}

void HUDLayer::OnAttach()
{

}

void HUDLayer::OnDetach()
{

}

void HUDLayer::OnUpdate(float dt)
{
	UISystem::Update(m_Registry);


	li::Renderer::BeginUI();

	UIRenderSystem::Render(m_Registry);

	li::Renderer::EndUI();
}

void HUDLayer::OnImGuiRender()
{

}

void HUDLayer::OnEvent(SDL_Event* event)
{
	UISystem::OnEvent(m_Registry, event);
}
