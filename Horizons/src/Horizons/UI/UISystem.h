#pragma once

#include "UIComponents.h"
#include "entt/entt.hpp"
#include "SDL.h"

class UISystem
{
public:
	static void Init(entt::registry& registry);
	static void Shutdown(entt::registry& registry);

	static void Update(entt::registry& registry);

	static void OnEvent(entt::registry& registry, SDL_Event* event);

	static const lay_id MaxItemCount = 1024u;

private:

	static void Recalculate(entt::registry& registry, cp::ui_context& context);

	static void Rebuild(entt::registry& registry, entt::entity context_ent);
	static void RebuildChildren(entt::registry& registry, cp::ui_context& context, cp::ui_element& element);
};