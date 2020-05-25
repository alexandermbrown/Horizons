#include "pch.h"
#include "PhysicsSystem.h"

#include "Horizons/Core/Core.h"
#include "PhysicsComponents.h"

void PhysicsSystem::Init(entt::registry& registry)
{
	entt::entity world = registry.create();
	cp::physics_world& pw = registry.emplace<cp::physics_world>(world);
	
	b2Vec2 gravity(0.0f, 0.0f);
	pw.world = new b2World(gravity);
}

void PhysicsSystem::Shutdown(entt::registry& registry)
{
	registry.view<cp::physics_world>().each([](cp::physics_world& world) {
		delete world.world;
	});
}

void PhysicsSystem::Step(entt::registry& registry, float dt)
{
	registry.view<cp::physics_world>().each([&](cp::physics_world& world) {
		world.world->Step(dt, HZ_PHYS_VELOCITY_ITERATIONS, HZ_PHYS_POSITION_ITERATIONS);
	});
}