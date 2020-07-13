#pragma once

#include "Sync.h"
#include "entt/entt.hpp"

#include "Lithium.h"

class SyncEventReceiveSystem
{
public:

	static void Init(entt::registry& registry);
	static void Update(entt::registry& registry, SyncEventQueue* queue);

private:

	template<typename component_t>
	static void AddComponent(entt::registry& registry, SyncEvent& event)
	{
		cp::sync_tracker& tracker = registry.ctx<cp::sync_tracker>();
		entt::entity entity = tracker.map[event.sync_id];
		LI_ASSERT(registry.get<cp::sync>(entity).sync_id == event.sync_id, "Sync IDs do not match!");

		if (event.data)
		{
			component_t* recieved_cp = (component_t*)event.data;
			registry.emplace<component_t>(entity, *recieved_cp);
			delete recieved_cp;
		}
		else
		{
			registry.emplace<component_t>(entity);
		}
	}

	template<typename component_t>
	static void RemoveComponent(entt::registry& registry, SyncEvent& event)
	{
		cp::sync_tracker& tracker = registry.ctx<cp::sync_tracker>();
		entt::entity entity = tracker.map[event.sync_id];
		LI_ASSERT(registry.get<cp::sync>(entity).sync_id == event.sync_id, "Sync IDs do not match!");

		registry.remove<component_t>(entity);
	}
};