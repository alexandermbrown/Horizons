#pragma once

#include "Horizons/Core/Core.h"

#ifdef HZ_PHYSICS_DEBUG_DRAW
#include "DebugPhysicsDrawShared.h"
#include "Lithium.h"

#include <queue>

class DebugPhysicsRenderer
{
public:
	DebugPhysicsRenderer();
	void Render(DebugDrawCommandQueue* queue);

private:
	constexpr static int MaxDrawVertices = 32768;
	constexpr static int MaxDrawIndices = MaxDrawVertices * 4;
	constexpr static float ZPos = 0.0f;

	struct DebugPhysicsVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	std::queue<DebugDrawCommand> m_CommandQueue;

	li::Ref<li::VertexArray> m_VertexArray;
	li::Ref<li::VertexBuffer> m_VertexBuffer;
	li::Ref<li::IndexBuffer> m_IndexBuffer;

	li::Ref<li::Shader> m_Shader;

	uint32_t m_VertexCount;
	uint32_t m_IndexCount;
	std::array<DebugPhysicsVertex, MaxDrawVertices> m_Vertices;
	std::array<uint32_t, MaxDrawIndices> m_Indices;
};

#endif