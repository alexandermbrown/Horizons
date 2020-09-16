#pragma once
#ifndef LI_DIST
#include "glm/glm.hpp"

struct Brush
{
	float InnerRadius;
	float OuterRadius;
	float Strength;
	bool Subtract;
	bool Enabled;
};
#endif