#pragma once
#ifndef LI_DIST
#include "glm/glm.hpp"

enum class VertexDisplayMode
{
	ShowInBrush = 0,
	Show,
	Hide
};

struct BrushSettings
{
	float InnerRadius;
	float OuterRadius;
	float Strength;
	bool Subtract;
	bool Enabled;
};

struct DisplaySettings
{
	bool ShowChunkBorders;
	VertexDisplayMode VertexDisplayMode;
};

struct EditorSettings
{
	BrushSettings Brush;
	DisplaySettings Display;
	int Layer;
};

#endif