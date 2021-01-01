#include "pch.h"
#include "SyncEventReceiveSystem.h"

#include "Horizons/Gameplay/TransformComponent.h"
#include "Horizons/Gameplay/Player/PlayerComponents.h"
#include "Horizons/Rendering/RenderingComponents.h"
#include "Horizons/Gameplay/Sync/SyncTransform.h"

#include "Lithium.h"

template<typename Component>
void AddComponent(entt::registry& registry, SyncEvent& event)
{
	cp::sync_tracker& tracker = registry.ctx<cp::sync_tracker>();
	entt::entity entity = tracker.map[event.sync_id];
	LI_ASSERT(registry.get<cp::sync>(entity).sync_id == event.sync_id, "Sync IDs do not match!");

	if (event.data)
	{
		Component* recieved_cp = static_cast<Component*>(event.data);
		registry.emplace<Component>(entity, *recieved_cp);
		delete recieved_cp;
	}
	else
	{
		registry.emplace<Component>(entity);
	}
}

template<typename Component>
void RemoveComponent(entt::registry& registry, SyncEvent& event)
{
	cp::sync_tracker& tracker = registry.ctx<cp::sync_tracker>();
	entt::entity entity = tracker.map[event.sync_id];
	LI_ASSERT(registry.get<cp::sync>(entity).sync_id == event.sync_id, "Sync IDs do not match!");

	registry.remove<Component>(entity);
}

void SyncEventReceiveSystem::Init(entt::registry& registry)
{
	cp::sync_tracker& tracker = registry.set<cp::sync_tracker>();
	tracker.map = std::unordered_map<uint64_t, entt::entity>();
}

void SyncEventReceiveSystem::Update(entt::registry& registry, SyncEventQueue* queue)
{
	SyncEvent event;
	while (queue->try_dequeue(event))
	{
		if (event.type == SyncType::CreateEntity)
		{
			entt::entity new_ent = registry.create();
			registry.emplace<cp::sync>(new_ent, event.sync_id);

			registry.ctx<cp::sync_tracker>().map[event.sync_id] = new_ent;

		}
		else if (event.type == SyncType::RemoveEntity)
		{
			cp::sync_tracker& tracker = registry.ctx<cp::sync_tracker>();
			entt::entity entity = tracker.map[event.sync_id];
			LI_ASSERT(registry.get<cp::sync>(entity).sync_id == event.sync_id, "Sync IDs do not match!");

			registry.destroy(entity);
			tracker.map.erase(event.sync_id);
		}
		else if (event.type == SyncType::AddComponent)
		{
			if (event.data1 == entt::type_info<cp::sync_transform>::id())
				AddComponent<cp::sync_transform>(registry, event);

			else if (event.data1 == entt::type_info<cp::transform>::id())
				AddComponent<cp::transform>(registry, event);

			else if (event.data1 == entt::type_info<cp::texture>::id())
				AddComponent<cp::texture>(registry, event);

			else if (event.data1 == entt::type_info<cp::color>::id())
				AddComponent<cp::color>(registry, event);

			else if (event.data1 == entt::type_info<cp::player>::id())
				AddComponent<cp::player>(registry, event);

			else LI_ERROR("Unknown component type {}", event.data1);
		}
		else if (event.type == SyncType::RemoveComponent)
		{
			if (event.data1 == entt::type_info<cp::sync_transform>::id())
				RemoveComponent<cp::sync_transform>(registry, event);

			else if (event.data1 == entt::type_info<cp::transform>::id())
				RemoveComponent<cp::transform>(registry, event);

			else if (event.data1 == entt::type_info<cp::texture>::id())
				RemoveComponent<cp::texture>(registry, event);

			else if (event.data1 == entt::type_info<cp::color>::id())
				RemoveComponent<cp::color>(registry, event);

			else if (event.data1 == entt::type_info<cp::player>::id())
				RemoveComponent<cp::player>(registry, event);

			else LI_ERROR("Unknown component type {}", event.data1);
		}
		else
		{
			LI_ERROR("Unknown sync event type!");
		}
	}
}
