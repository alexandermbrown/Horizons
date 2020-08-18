#pragma once

#include "UIComponents.h"
#include "entt/entt.hpp"
#include "SDL.h"

class UILayoutSystem
{
public:
	static void Init(entt::registry& registry);
	static void Shutdown(entt::registry& registry);

	static void Update(entt::registry& registry);

	static void OnWindowResize(entt::registry& registry, int width, int height);


	static void AddChild(entt::registry& registry, entt::entity parent, entt::entity child);
	static void AddSibling(entt::registry& registry, entt::entity existing, entt::entity new_ent);


	static const lay_id MaxItemCount = 1024u;

private:

	static void Recalculate(entt::registry& registry, cp::ui_context& context);

	static void Rebuild(entt::registry& registry, entt::entity context_ent);
	static void RebuildChildren(entt::registry& registry, cp::ui_context& context, cp::ui_element& element, float current_z, float current_range);
};