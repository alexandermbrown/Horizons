#pragma once

#include "box2d/box2d.h"
#include "Horizons/Core/Core.h"

#ifdef HZ_PHYSICS_DEBUG_DRAW
#include "DebugDrawSystem.h"
#endif

#include "Lithium.h"

namespace cp
{
	struct physics_world
	{
		Li::Unique<b2World> world;
	};

	struct physics_body
	{
		b2Body* body;
	};

#ifdef HZ_PHYSICS_DEBUG_DRAW
	struct physics_debug_draw
	{
		PhysicsDebugDraw* draw;
	};
#endif
}