#pragma once

#include "UIComponents.h"
#include "Lithium.h"
#include "entt/entt.hpp"
#include "SDL.h"

namespace Systems::UILayout
{
	void OnDeinit(entt::registry& registry);
	void OnUpdate(entt::registry& registry, Li::Duration::us dt_unused);
	void OnEvent(entt::registry& registry, SDL_Event* event);

	void AddChild(entt::registry& registry, entt::entity parent, entt::entity child);
	void AddSibling(entt::registry& registry, entt::entity existing, entt::entity new_ent);
};
