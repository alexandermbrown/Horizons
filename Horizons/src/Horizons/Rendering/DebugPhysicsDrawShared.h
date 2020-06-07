#pragma once

#include "glm/glm.hpp"
#include "readerwriterqueue/readerwriterqueue.h"

enum class DebugDrawType
{
	End,
	Point,
	Line,
	PolygonOutline,
	Circle
};

struct DebugDrawCommand
{
	DebugDrawType Type;
	glm::vec4 Color;

	glm::vec2* Vertices;
	uint32_t VertexCount;

	glm::vec2 Point1;
	glm::vec2 Point2;

	float Radius;
};

using DebugDrawCommandQueue = moodycamel::ReaderWriterQueue<DebugDrawCommand>;