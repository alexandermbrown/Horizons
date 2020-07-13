#include "pch.h"
#include "SyncTransformSendSystem.h"

#include "Horizons/Gameplay/Physics/PhysicsComponents.h"

void SyncTransformSendSystem::Update(entt::registry& registry, SyncTransformQueue* queue)
{
	registry.view<cp::sync_transform, cp::physics_body>().each([queue](cp::sync_transform& sync_transform, cp::physics_body& body)
	{
		cp::sync_transform transform;

		transform.position = { body.body->GetPosition().x, body.body->GetPosition().y };
		transform.velocity = { body.body->GetLinearVelocity().x, body.body->GetLinearVelocity().y };

		transform.rotation = body.body->GetAngle();
		transform.angular_velocity = body.body->GetAngularVelocity();

		if (transform != sync_transform)
		{
			sync_transform = transform;
			queue->enqueue(sync_transform);
		}
	});
}
