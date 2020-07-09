#include "pch.h"
#include "PlayerSystem.h"

#include "PlayerComponents.h"
#include "Horizons/Gameplay/Physics/PhysicsComponents.h"

namespace PlayerSystem
{
	void Init(entt::registry& registry)
	{
		entt::entity player = registry.create();
		registry.emplace<cp::player>(player);
		
		cp::physics_body& physBody = registry.emplace<cp::physics_body>(player);

		// Get physics world from the registry.
		auto worldView = registry.view<cp::physics_world>();
		auto& world = worldView.get(worldView.front());

		// Create the player's physics body.
		b2BodyDef playerBodyDef;
		playerBodyDef.type = b2_dynamicBody;
		playerBodyDef.position.Set(0.0f, 0.0f); // TODO: define where the player is in the world.
		physBody.body = world.world->CreateBody(&playerBodyDef);

		
		b2CircleShape playerShape;
		playerShape.m_p.Set(0.0f, 0.0f);
		playerShape.m_radius = 0.5f;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &playerShape;
		fixtureDef.density = 1.0f; // TODO: Set player density.
		fixtureDef.friction = 0.3f; // TODO: Set player friction.
		fixtureDef.restitution = 0.0f; // TODO: Set player restitution.

		physBody.body->CreateFixture(&fixtureDef);

		physBody.body->ApplyAngularImpulse(0.1f, true);
		physBody.body->ApplyLinearImpulseToCenter({ 0.5f, 0.1f }, true);
	}
}