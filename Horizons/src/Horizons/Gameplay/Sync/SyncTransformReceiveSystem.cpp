#include "pch.h"
#include "SyncTransformReceiveSystem.h"

#include "Horizons/Gameplay/Components.h"
#include "Horizons/Gameplay/Sync/Sync.h"

#include "Lithium.h"

void AddSyncTransform(entt::registry& registry, entt::entity entity)
{
	cp::sync_transform& sync_transform = registry.get<cp::sync_transform>(entity);
	cp::transform& transform = registry.emplace<cp::transform>(entity);

	transform.position = { sync_transform.position.x, sync_transform.position.y, sync_transform.position.z };
	transform.rotation = sync_transform.rotation;

	transform.old = true;
}

void SyncTransformReceiveSystem::Init(entt::registry& registry)
{
	registry.on_construct<cp::sync_transform>().connect<&AddSyncTransform>();
}

void SyncTransformReceiveSystem::Update(entt::registry& registry, SyncTransformQueue* queue, float dt)
{
	// Possible Improvement: Include a timestamp in the queue to better synchronize.

	cp::sync_transform received;
	while (queue->try_dequeue(received))
	{
		entt::entity entity = registry.ctx<cp::sync_tracker>().map.at(received.sync_id);
		registry.emplace_or_replace<cp::sync_transform>(entity, received);

		if (registry.has<cp::transform>(entity))
		{
			cp::transform& transform = registry.get<cp::transform>(entity);
			transform.position = { received.position.x, received.position.y, received.position.z };
			transform.rotation = received.rotation;

			transform.old = true;
		}
	}

	registry.view<cp::sync_transform, cp::transform>().each([dt](cp::sync_transform& sync, cp::transform& transform)
	{
		if (sync.velocity.x || sync.velocity.y || sync.angular_velocity)
		{
			transform.position.x += sync.velocity.x * dt;
			transform.position.y += sync.velocity.y * dt;
			transform.rotation += sync.angular_velocity * dt;

			transform.old = true;
		}
	});
}