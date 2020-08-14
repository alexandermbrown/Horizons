#include "pch.h"
#include "HUDLayer.h"

#include "Horizons/Rendering/RenderingComponents.h"
#include "Horizons/UI/UIComponents.h"
#include "Horizons/UI/UISystem.h"
#include "Horizons/UI/UIRenderSystem.h"
#include "Horizons/UI/UITransformSortSystem.h"
#include "Horizons/UI/UIHoverSystem.h"
#include "Horizons/UI/UIClickSystem.h"

#include "imgui.h"

HUDLayer::HUDLayer()
	: m_Registry()
{
	UILayoutSystem::Init(m_Registry);

	entt::entity context_ent = m_Registry.view<cp::ui_context>().front();

	entt::entity item_bar = m_Registry.create();

	auto& bar_element = m_Registry.emplace<cp::ui_element>(item_bar);
	bar_element.layout_contain = LAY_ROW;
	bar_element.layout_behave = LAY_BOTTOM | LAY_HFILL;
	bar_element.margins = lay_vec4_xyzw(30, 0, 30, 20);
	bar_element.height = 70;

	m_Registry.emplace<cp::ui_transform>(item_bar, 0.201f);
	m_Registry.emplace<cp::color>(item_bar, glm::vec4(0.4f, 0.4f, 0.4f, 0.6f));

	UILayoutSystem::AddChild(m_Registry, context_ent, item_bar);

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
		
		auto& hover = m_Registry.emplace<cp::ui_hover>(item_slot);
		hover.OnMouseEnterFn = [](entt::registry& registry, entt::entity entity)
		{
			auto& color = registry.get<cp::color>(entity);
			color.color = { 0.8f, 0.6f, 0.5f, 1.0f };
		};

		hover.OnMouseLeaveFn = [](entt::registry& registry, entt::entity entity)
		{
			auto& color = registry.get<cp::color>(entity);
			color.color = { 0.7f, 0.5f, 0.4f, 0.7f };
		};

		auto& click = m_Registry.emplace<cp::ui_click>(item_slot);
		click.OnClickFn = [](entt::registry& registry, entt::entity entity, int button) -> bool
		{
			LI_TRACE("Click on entity {} with button {}", entt::registry::entity(entity), button);
			return true;
		};

		UILayoutSystem::AddChild(m_Registry, item_bar, item_slot);
	}

	{
		li::Ref<li::Label> label = li::CreateRef<li::Label>(std::u16string(u"The Server is the main part, used to receive and send messages. The ClientServerConfig allows you to configure the server: connection timeout, memory used or the different channels (reliable, unreliable). "), 26, li::ResourceManager::Get<li::Font>("Lora-Regular"));

		entt::entity label_ent = m_Registry.create();
		auto& element = m_Registry.emplace<cp::ui_element>(label_ent);
		element.layout_behave = LAY_TOP | LAY_LEFT;
		element.layout_contain = LAY_LAYOUT;
		element.width = 1;
		element.height = 1;
		element.margins = lay_vec4_xyzw(60, 260, 60, 60);

		m_Registry.emplace<cp::ui_transform>(label_ent, 0.2015f);
		auto& cp_label = m_Registry.emplace<cp::label>(label_ent);
		cp_label.label_ref = label;

		UILayoutSystem::AddChild(m_Registry, context_ent, label_ent);
	}

	cp::ui_context& context = m_Registry.get<cp::ui_context>(context_ent);
	context.rebuild = true;
	context.recalculate = true;
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
