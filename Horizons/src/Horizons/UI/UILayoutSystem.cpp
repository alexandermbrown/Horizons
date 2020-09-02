#include "pch.h"
#include "UILayoutSystem.h"

#include "Horizons/Gameplay/Components.h"

#include "glm/gtc/matrix_transform.hpp"

#include "Lithium.h"

void UILayoutSystem::Init(entt::registry& registry)
{
	entt::entity ui_context = registry.create();
	cp::ui_context& context = registry.emplace<cp::ui_context>(ui_context);
	context.rebuild = true;
	context.recalculate = true;

	registry.emplace<cp::ui_element>(ui_context);

	lay_init_context(&context.context);
	lay_reserve_items_capacity(&context.context, MaxItemCount);
}

void UILayoutSystem::Shutdown(entt::registry& registry)
{
	auto& view = registry.view<cp::ui_context>();
	view.each([](cp::ui_context& context) {
		lay_destroy_context(&context.context);
	});

	registry.destroy(view.begin(), view.end());
}

void UILayoutSystem::Update(entt::registry& registry)
{
	entt::entity context_ent = registry.view<cp::ui_context>().front();
	cp::ui_context& context = registry.get<cp::ui_context>(context_ent);

	if (context.rebuild)
	{
		Rebuild(registry, context_ent);
		Recalculate(registry, context);
		context.rebuild = false;
		context.recalculate = false;
	}
	else if (context.recalculate)
	{
		Recalculate(registry, context);
		context.recalculate = false;
	}
}

void UILayoutSystem::OnWindowResize(entt::registry& registry, int width, int height)
{
	registry.view<cp::ui_context, cp::ui_element>().each([width, height](cp::ui_context& context, cp::ui_element& element)
	{
		lay_set_size_xy(&context.context, element.layout_id, (lay_scalar)width, (lay_scalar)height);
		context.recalculate = true;
	});
}

void UILayoutSystem::AddChild(entt::registry& registry, entt::entity parent, entt::entity child)
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
	parent_ui.num_children++;
}

void UILayoutSystem::AddSibling(entt::registry& registry, entt::entity existing, entt::entity new_ent)
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

void UILayoutSystem::Rebuild(entt::registry& registry, entt::entity context_ent)
{
	LI_TRACE("Rebuilding UI...");

	cp::ui_context& context = registry.get<cp::ui_context>(context_ent);
	cp::ui_element& root = registry.get<cp::ui_element>(context_ent);

	lay_reset_context(&context.context);

	root.layout_id = lay_item(&context.context);

	li::Window* window = li::Application::Get()->GetWindow();
	lay_set_size_xy(&context.context, root.layout_id, (lay_scalar)window->GetWidth(), (lay_scalar)window->GetHeight());

	RebuildChildren(registry, context, root, context.start_z, context.z_range);
}

void UILayoutSystem::RebuildChildren(entt::registry& registry, cp::ui_context& context, cp::ui_element& parent_element, float current_z, float current_range)
{
	int index = 0;
	float step = current_range / (float)(parent_element.num_children + 1);
	for (entt::entity child = parent_element.first_child; registry.valid(child); child = registry.get<cp::ui_element>(child).next_sibling)
	{
		current_z += step;

		cp::ui_element& child_element = registry.get<cp::ui_element>(child);
		child_element.layout_id = lay_item(&context.context);
		child_element.z = current_z;

		lay_insert(&context.context, parent_element.layout_id, child_element.layout_id);

		if (child_element.width || child_element.height)
			lay_set_size_xy(&context.context, child_element.layout_id, child_element.width, child_element.height);

		if (child_element.margins[0] || child_element.margins[1] || child_element.margins[2] || child_element.margins[3])
			lay_set_margins(&context.context, child_element.layout_id, child_element.margins);

		if (child_element.layout_behave)
			lay_set_behave(&context.context, child_element.layout_id, child_element.layout_behave);

		if (child_element.layout_contain)
			lay_set_contain(&context.context, child_element.layout_id, child_element.layout_contain);

		RebuildChildren(registry, context, child_element, current_z, step);
		index++;
	}
	LI_ASSERT(index == parent_element.num_children, "Invalid number of children.");
}

void UILayoutSystem::Recalculate(entt::registry& registry, cp::ui_context& context)
{
	LI_TRACE("Recalculating UI...");

	lay_run_context(&context.context);

	registry.view<cp::ui_transform, cp::ui_element>().each([&](entt::entity entity, cp::ui_transform& transform, cp::ui_element& ui_element)
	{
		lay_vec4 rect = lay_get_rect(&context.context, ui_element.layout_id);

		transform.transform = glm::translate(glm::mat4(1.0f), { (float)rect[0],
			(float)(li::Application::Get()->GetWindow()->GetHeight() - rect[1] - rect[3]), ui_element.z })
			* glm::scale(glm::mat4(1.0f), { (float)rect[2], (float)rect[3], 1.0f });
	});
}
