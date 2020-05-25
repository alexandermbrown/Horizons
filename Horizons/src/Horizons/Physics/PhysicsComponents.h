#pragma once

#include "box2d/box2d.h"

namespace cp
{
	struct physics_world
	{
		b2World* world;
	};

	struct physics_body
	{
		b2Body* body;
	};
}