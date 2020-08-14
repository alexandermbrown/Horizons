#include "pch.h"
#include "Math.h"

#include "Lithium.h"

bool Math::TransformContainsPoint(const glm::mat4& transform, int x, int y)
{
	float left = transform[3][0];
	float right = left + transform[0][0];
	float bottom = transform[3][1];
	float top = bottom + transform[1][1];

	y = li::Application::Get()->GetWindow()->GetHeight() - y;
	return x >= left && x <= right && y >= bottom && y <= top;
}
