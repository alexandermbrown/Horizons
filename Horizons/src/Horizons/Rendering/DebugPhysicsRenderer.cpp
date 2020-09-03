#include "pch.h"
#include "DebugPhysicsRenderer.h"

#include <math.h>

#ifdef HZ_PHYSICS_DEBUG_DRAW

DebugPhysicsRenderer::DebugPhysicsRenderer(DebugDrawCommandQueue* queue)
	: m_ThreadQueue(queue), m_VertexCount(0), m_IndexCount(0)
{
	m_Shader = li::ResourceManager::Get<li::Shader>("shader_debug_physics");

	m_VertexArray = li::VertexArray::Create();

	// Ensure the vertex struct is packed so it can be safely sent the GPU.
	static_assert(sizeof(DebugPhysicsVertex) == sizeof(glm::vec3) + sizeof(glm::vec4));
	static_assert(sizeof(m_Vertices) == (sizeof(float) * 3 + sizeof(float) * 4) * MaxDrawVertices);

	m_VertexBuffer = li::VertexBuffer::Create(MaxDrawVertices * sizeof(DebugPhysicsVertex), li::BufferUsage::DynamicDraw);
	m_VertexBuffer->SetLayout({
		{ li::ShaderDataType::Float3, "POSITION", 0 },
		{ li::ShaderDataType::Float4, "COLOR", 1 }
	});

	m_IndexBuffer = li::IndexBuffer::Create(MaxDrawIndices * (uint32_t)(sizeof uint32_t), li::BufferUsage::DynamicDraw);

	m_VertexArray->AddVertexBuffer(m_VertexBuffer);
	m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	m_VertexArray->Finalize(m_Shader);

	m_Shader->AddUniformBuffer(li::Renderer::GetViewProjUniformBuffer());
}

void DebugPhysicsRenderer::Render()
{
	DebugDrawCommand command;
	while (m_ThreadQueue->try_dequeue(command)) // TODO: check if there are multiple ENDs in the queue, only render most recent.
	{
		m_CommandQueue.push(command);
	}

	if (m_CommandQueue.size() > 0 && m_CommandQueue.back().Type == DebugDrawType::End)
	{
		// Flush queue then set the new vertices in the vertex array for rendering later.

		m_VertexCount = 0;
		m_IndexCount = 0;

		while (m_CommandQueue.size() > 0ULL)
		{
			DebugDrawCommand& command = m_CommandQueue.front();

			if (command.Type == DebugDrawType::End)
			{
				m_CommandQueue.pop(); // TODO: check that this is OK.
				break;
			}

			switch (command.Type)
			{
			case DebugDrawType::Point:
				// Draw a '+' with two lines to represent the point.
				m_Vertices[m_VertexCount].Position = { command.Point1.x - 0.01, command.Point1.y, ZPos };
				m_Vertices[m_VertexCount].Color = command.Color;
				m_Indices[m_IndexCount] = m_VertexCount;
				m_VertexCount++;
				m_IndexCount++;

				m_Vertices[m_VertexCount].Position = { command.Point1.x + 0.01, command.Point1.y, ZPos };
				m_Vertices[m_VertexCount].Color = command.Color;
				m_Indices[m_IndexCount] = m_VertexCount;
				m_VertexCount++;
				m_IndexCount++;

				m_Vertices[m_VertexCount].Position = { command.Point1.x, command.Point1.y - 0.01, ZPos };
				m_Vertices[m_VertexCount].Color = command.Color;
				m_Indices[m_IndexCount] = m_VertexCount;
				m_VertexCount++;
				m_IndexCount++;

				m_Vertices[m_VertexCount].Position = { command.Point1.x, command.Point1.y + 0.01, ZPos };
				m_Vertices[m_VertexCount].Color = command.Color;
				m_Indices[m_IndexCount] = m_VertexCount;
				m_VertexCount++;
				m_IndexCount++;

				break;
			case DebugDrawType::Line:
				// Draw a line with the two vertices.
				m_Vertices[m_VertexCount].Position = { command.Point1, ZPos };
				m_Vertices[m_VertexCount].Color = command.Color;
				m_Indices[m_IndexCount] = m_VertexCount;
				m_VertexCount++;
				m_IndexCount++;

				m_Vertices[m_VertexCount].Position = { command.Point2, ZPos };
				m_Vertices[m_VertexCount].Color = command.Color;
				m_Indices[m_IndexCount] = m_VertexCount;
				m_VertexCount++;
				m_IndexCount++;

				break;
			case DebugDrawType::PolygonOutline:
			{
				uint32_t firstIndex = m_VertexCount;

				LI_ASSERT(command.VertexCount > 1, "Polygons must have more than 1 vertice!");

				for (uint32_t i = 0; i < command.VertexCount; i++)
				{

					m_Vertices[m_VertexCount].Position = { command.Vertices[i], ZPos };
					m_Vertices[m_VertexCount].Color = command.Color;

					m_Indices[m_IndexCount] = m_VertexCount;
					m_IndexCount++;
					m_Indices[m_IndexCount] = (i == command.VertexCount - 1) ? firstIndex : m_VertexCount + 1;
					m_IndexCount++;

					m_VertexCount++;
				}

				delete[] command.Vertices;
				break;
			}
			case DebugDrawType::Circle:
			{
				uint32_t firstIndex = m_VertexCount;

				constexpr int resolution = 32;
				for (int i = 0; i < resolution; i++)
				{
					float angle = (float)i / (float)resolution * 2.0f * (float)M_PI;
					
					m_Vertices[m_VertexCount].Position = {
						std::cos(angle) * command.Radius + command.Point1.x,
						std::sin(angle) * command.Radius + command.Point1.y,
						ZPos
					};
					m_Vertices[m_VertexCount].Color = command.Color;

					m_Indices[m_IndexCount] = m_VertexCount;
					m_IndexCount++;
					m_Indices[m_IndexCount] = (i == resolution - 1) ? firstIndex : m_VertexCount + 1;
					m_IndexCount++;

					m_VertexCount++;
				}

				break;
			}
			}

			m_CommandQueue.pop();
		}

		m_VertexBuffer->SetSubData((float*)m_Vertices.data(), m_VertexCount * sizeof(DebugPhysicsVertex), 0, true);
		m_IndexBuffer->SetSubData(m_Indices.data(), m_IndexCount * sizeof(uint32_t), 0, true);
	}

	// Render the lines.
	if (m_IndexCount > 0)
	{
		m_Shader->Bind();
		m_VertexArray->Bind();
		li::Application::Get()->GetWindow()->GetContext()->SetDrawMode(li::DrawMode::Lines);
		li::Application::Get()->GetWindow()->GetContext()->DrawIndexed(m_IndexCount);
		m_VertexArray->Unbind();
	}
}

#endif