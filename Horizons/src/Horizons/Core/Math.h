#pragma once

#include "glm/glm.hpp"

class Math
{
public:
	static bool TransformContainsPoint(const glm::mat4& transform, int x, int y);
};