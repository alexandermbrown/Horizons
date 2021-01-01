#include "pch.h"
#include "Math.h"

#include "Lithium.h"
#include <cmath>

bool Math::TransformContainsPoint(const glm::mat4& transform, int x, int y)
{
	float left = transform[3][0];
	float right = left + transform[0][0];
	float bottom = transform[3][1];
	float top = bottom + transform[1][1];

	y = Li::Application::Get().GetWindow().GetHeight() - y;
	return x >= left && x <= right && y >= bottom && y <= top;
}

int Math::PositiveMod(int a, int b)
{
	return ((a % b) + b) % b;
}

float Math::PositiveMod(float a, float b)
{
	return std::fmod(std::fmod(a, b) + b, b);
}
