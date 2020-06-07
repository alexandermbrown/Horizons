#pragma once

#include "Horizons/Core/Core.h"

#ifdef HZ_PHYSICS_DEBUG_DRAW
#include "DebugPhysicsDrawShared.h"
#include "Lithium.h"

#include <queue>

#define HZ_PHYSICS_DEBUG_DRAW_MAX_VERTICES 32768U
#define HZ_PHYSICS_DEBUG_DRAW_MAX_INDICES HZ_PHYSICS_DEBUG_DRAW_MAX_VERTICES * 4

class DebugPhysicsRenderer
{
public:

	DebugPhysicsRenderer(DebugDrawCommandQueue* queue);

	void Render(li::OrthographicCamera* camera, float z);

private:

	struct DebugPhysicsVertex
	{
		glm::vec2 Position;
		glm::vec4 Color;
	};

	void RenderExisting(li::OrthographicCamera* camera, float z);

	DebugDrawCommandQueue* m_ThreadQueue;
	std::queue<DebugDrawCommand> m_CommandQueue;

	li::Ref<li::VertexArray> m_VertexArray;
	li::Ref<li::VertexBuffer> m_VertexBuffer;
	li::Ref<li::IndexBuffer> m_IndexBuffer;

	uint32_t m_VertexCount;
	uint32_t m_IndexCount;
	std::array<DebugPhysicsVertex, HZ_PHYSICS_DEBUG_DRAW_MAX_VERTICES> m_Vertices;
	std::array<uint32_t, HZ_PHYSICS_DEBUG_DRAW_MAX_INDICES> m_Indices;
};

#endif