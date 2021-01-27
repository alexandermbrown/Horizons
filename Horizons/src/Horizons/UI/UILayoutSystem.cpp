#include "pch.h"
#include "UILayoutSystem.h"

#include "Horizons/Gameplay/TransformComponent.h"

#include "Lithium.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Systems::UILayout
{
	void Recalculate(entt::registry& registry, cp::ui_context& context);
	void Rebuild(entt::registry& registry, entt::entity context_ent);
	void RebuildChildren(entt::registry& registry, cp::ui_context& context, cp::ui_element& element, float current_z, float current_range);
}

void Systems::UILayout::OnDeinit(entt::registry& registry)
{
	auto& view = registry.view<cp::ui_context>();
	registry.destroy(view.begin(), view.end());
}

void Systems::UILayout::OnUpdate(entt::registry& registry, Li::Duration::us dt_unused)
{
	entt::entity context_ent = registry.view<cp::ui_context>().front();
	auto& context = registry.get<cp::ui_context>(context_ent);

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

void Systems::UILayout::OnEvent(entt::registry& registry, SDL_Event* event)
{
	if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED
		&& event->window.windowID == Li::Application::Get().GetWindow().GetWindowID())
	{
		registry.view<cp::ui_context, cp::ui_element>().each([event](cp::ui_context& context, cp::ui_element& element)
		{
			lay_set_size_xy(context.context.get(), element.layout_id, (lay_scalar)event->window.data1, (lay_scalar)event->window.data2);
			context.recalculate = true;
		});
	}
}

void Systems::UILayout::AddChild(entt::registry& registry, entt::entity parent, entt::entity child)
{
	cp::ui_element& parent_ui = registry.get<cp::ui_element>(parent);
	cp::ui_element& child_ui = registry.get<cp::ui_element>(child);

	if (parent_ui.first_child == entt::null)
		parent_ui.first_child = child;
	else
		AddSibling(registry, parent_ui.first_child, child);

	child_ui.parent = parent;
	parent_ui.num_children++;
}

void Systems::UILayout::AddSibling(entt::registry& registry, entt::entity existing_sibling, entt::entity new_sibling)
{
	entt::entity current = existing_sibling;
	entt::entity prev = entt::null;
	while (current != entt::null)
	{
		prev = current;
		current = registry.get<cp::ui_element>(current).next_sibling;
	}

	if (prev != entt::null)
	{
		cp::ui_element& existing_ui = registry.get<cp::ui_element>(prev);
		existing_ui.next_sibling = new_sibling;
	}
	else LI_ASSERT(false, "Existing sibling is null");
}

void Systems::UILayout::Rebuild(entt::registry& registry, entt::entity context_ent)
{
	Li::Log::Trace("Rebuilding UI...");

	cp::ui_context& context = registry.get<cp::ui_context>(context_ent);
	cp::ui_element& root = registry.get<cp::ui_element>(context_ent);

	lay_reset_context(context.context.get());

	root.layout_id = lay_item(context.context.get());

	Li::Window& window = Li::Application::Get().GetWindow();
	lay_set_size_xy(context.context.get(), root.layout_id, (lay_scalar)window.GetWidth(), (lay_scalar)window.GetHeight());

	RebuildChildren(registry, context, root, context.start_z, context.z_range);
}

void Systems::UILayout::RebuildChildren(entt::registry& registry, cp::ui_context& context, cp::ui_element& parent_element, float current_z, float current_range)
{
	int index = 0;
	float step = current_range / (float)(parent_element.num_children + 1);
	for (entt::entity child = parent_element.first_child; registry.valid(child); child = registry.get<cp::ui_element>(child).next_sibling)
	{
		current_z += step;

		cp::ui_element& child_element = registry.get<cp::ui_element>(child);
		child_element.layout_id = lay_item(context.context.get());
		child_element.z = current_z;

		lay_insert(context.context.get(), parent_element.layout_id, child_element.layout_id);

		if (child_element.width || child_element.height)
			lay_set_size_xy(context.context.get(), child_element.layout_id, child_element.width, child_element.height);

		if (child_element.margins[0] || child_element.margins[1] || child_element.margins[2] || child_element.margins[3])
			lay_set_margins(context.context.get(), child_element.layout_id, child_element.margins);

		if (child_element.layout_behave)
			lay_set_behave(context.context.get(), child_element.layout_id, child_element.layout_behave);

		if (child_element.layout_contain)
			lay_set_contain(context.context.get(), child_element.layout_id, child_element.layout_contain);

		RebuildChildren(registry, context, child_element, current_z, step);
		index++;
	}
	LI_ASSERT(index == parent_element.num_children, "Invalid number of children.");
}

void Systems::UILayout::Recalculate(entt::registry& registry, cp::ui_context& context)
{
	Li::Log::Trace("Recalculating UI...");

	lay_run_context(context.context.get());

	registry.view<cp::ui_transform, cp::ui_element>().each([&](entt::entity entity, cp::ui_transform& transform, cp::ui_element& ui_element)
	{
		lay_vec4 rect = lay_get_rect(context.context.get(), ui_element.layout_id);

		transform.transform = glm::translate(glm::mat4(1.0f), { (float)rect[0],
			(float)(Li::Application::Get().GetWindow().GetHeight() - rect[1] - rect[3]), ui_element.z })
			* glm::scale(glm::mat4(1.0f), { (float)rect[2], (float)rect[3], 1.0f });
	});
}
