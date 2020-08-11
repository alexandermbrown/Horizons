#include "pch.h"
#include "UISystem.h"

#include "Horizons/Gameplay/Components.h"

#include "glm/gtc/matrix_transform.hpp"

#include "Lithium.h"

void UISystem::Init(entt::registry& registry)
{
	entt::entity ui_context = registry.create();
	cp::ui_context& context = registry.emplace<cp::ui_context>(ui_context);
	context.rebuild = true;
	context.recalculate = true;

	registry.emplace<cp::ui_element>(ui_context);

	lay_init_context(&context.context);
	lay_reserve_items_capacity(&context.context, MaxItemCount);
}

void UISystem::Shutdown(entt::registry& registry)
{
	auto& view = registry.view<cp::ui_context>();
	view.each([](cp::ui_context& context) {
		lay_destroy_context(&context.context);
		});

	registry.destroy(view.begin(), view.end());
}

void UISystem::Update(entt::registry& registry)
{
	entt::entity context_ent = registry.view<cp::ui_context>().front();
	cp::ui_context& context = registry.get<cp::ui_context>(context_ent);

	if (context.rebuild)
	{
		Rebuild(registry, context_ent);
		context.rebuild = false;
		LI_ASSERT(context.recalculate == true, "You must recalculate when rebuilding UI.");
	}

	if (context.recalculate)
	{
		Recalculate(registry, context);
		context.recalculate = false;
	}
}

void UISystem::OnEvent(entt::registry& registry, SDL_Event* event)
{
	if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
	{
		registry.view<cp::ui_context, cp::ui_element>().each([](cp::ui_context& context, cp::ui_element& element)
			{
				li::Window* window = li::Application::Get()->GetWindow();
				lay_set_size_xy(&context.context, element.layout_id, (lay_scalar)window->GetWidth(), (lay_scalar)window->GetHeight());
				context.recalculate = true;
			});
	}
}

void UISystem::AddChild(entt::registry& registry, entt::entity parent, entt::entity child)
{
	cp::ui_element& parent_ui = registry.get<cp::ui_element>(parent);
	cp::ui_element& child_ui = registry.get<cp::ui_element>(child);

	if (parent_ui.first_child == entt::null)
	{
		parent_ui.first_child = child;
	}
	else
	{
		AddSibling(registry, parent_ui.first_child, child);
	}

	child_ui.parent = parent;
}

void UISystem::AddSibling(entt::registry& registry, entt::entity existing, entt::entity new_ent)
{
	cp::ui_element& existing_ui = registry.get<cp::ui_element>(existing);

	if (existing_ui.next_sibling == entt::null)
	{
		existing_ui.next_sibling = new_ent;
	}
	else
	{
		AddSibling(registry, existing_ui.next_sibling, new_ent);
	}
}

void UISystem::Rebuild(entt::registry& registry, entt::entity context_ent)
{
	LI_TRACE("Rebuilding UI...");

	cp::ui_context& context = registry.get<cp::ui_context>(context_ent);
	cp::ui_element& root = registry.get<cp::ui_element>(context_ent);

	lay_reset_context(&context.context);

	root.layout_id = lay_item(&context.context);

	RebuildChildren(registry, context, root);
}

void UISystem::RebuildChildren(entt::registry& registry, cp::ui_context& context, cp::ui_element& parent_element)
{
	for (entt::entity child = parent_element.first_child; registry.valid(child); child = registry.get<cp::ui_element>(child).next_sibling)
	{
		cp::ui_element& child_element = registry.get<cp::ui_element>(child);
		child_element.layout_id = lay_item(&context.context);
		lay_insert(&context.context, parent_element.layout_id, child_element.layout_id);

		if (child_element.width || child_element.height)
			lay_set_size_xy(&context.context, child_element.layout_id, child_element.width, child_element.height);

		if (child_element.margins[0] || child_element.margins[1] || child_element.margins[2] || child_element.margins[3])
			lay_set_margins(&context.context, child_element.layout_id, child_element.margins);

		if (child_element.layout_behave)
			lay_set_behave(&context.context, child_element.layout_id, child_element.layout_behave);

		if (child_element.layout_contain)
			lay_set_contain(&context.context, child_element.layout_id, child_element.layout_contain);

		RebuildChildren(registry, context, child_element);
	}
}

void UISystem::Recalculate(entt::registry& registry, cp::ui_context& context)
{
	LI_TRACE("Recalculating UI...");

	lay_run_context(&context.context);

	registry.view<cp::ui_transform, cp::ui_element>().each([&](cp::ui_transform& transform, cp::ui_element& ui_element)
		{
			lay_vec4 rect = lay_get_rect(&context.context, ui_element.layout_id);

			transform.transform = glm::translate(glm::mat4(1.0f), { (float)rect[0],
				(float)(li::Application::Get()->GetWindow()->GetHeight() - rect[1] - rect[3]), transform.z })
				* glm::scale(glm::mat4(1.0f), { (float)rect[2], (float)rect[3], 1.0f });
		});
}
