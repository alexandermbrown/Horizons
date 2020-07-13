#include "pch.h"
#include "PhysicsSystem.h"

#include "Horizons/Core/Core.h"
#include "PhysicsComponents.h"

void PhysicsSystem::Init(entt::registry& registry)
{
	cp::physics_world& pw = registry.set<cp::physics_world>();
	
	b2Vec2 gravity(0.0f, 0.0f);
	pw.world = new b2World(gravity);
}

void PhysicsSystem::Shutdown(entt::registry& registry)
{
	delete registry.ctx<cp::physics_world>().world;
}

void PhysicsSystem::Step(entt::registry& registry, float dt)
{
	registry.ctx<cp::physics_world>().world->Step(dt, HZ_PHYS_VELOCITY_ITERATIONS, HZ_PHYS_POSITION_ITERATIONS);
}