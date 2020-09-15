#pragma once

#include "glm/glm.hpp"

namespace Math
{
	bool TransformContainsPoint(const glm::mat4& transform, int x, int y);
	int PositiveMod(int a, int b);
	float PositiveMod(float a, float b);
};