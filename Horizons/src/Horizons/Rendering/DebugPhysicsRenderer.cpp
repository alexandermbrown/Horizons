#include "pch.h"
#include "DebugPhysicsRenderer.h"

#ifdef HZ_PHYSICS_DEBUG_DRAW

DebugPhysicsRenderer::DebugPhysicsRenderer(DebugDrawCommandQueue* queue)
	: m_ThreadQueue(queue), m_VertexCount(0), m_IndexCount(0)
{
	m_VertexArray = li::VertexArray::Create();

	li::BufferLayout layout = {
		{ li::ShaderDataType::Float2, "a_Position", 0 },
		{ li::ShaderDataType::Float4, "a_Color", 1 }
	};

	// Ensure the vertex struct is packed so it can be safely sent the GPU.
	static_assert(sizeof(DebugPhysicsVertex) == sizeof(glm::vec2) + sizeof(glm::vec4), "Vertex struct not packed!");

	m_VertexBuffer = li::VertexBuffer::Create(HZ_PHYSICS_DEBUG_DRAW_MAX_VERTICES * sizeof(DebugPhysicsVertex), li::BufferUsage::DynamicDraw);
	m_VertexBuffer->SetLayout(layout);

	m_IndexBuffer = li::IndexBuffer::Create(HZ_PHYSICS_DEBUG_DRAW_MAX_INDICES * static_cast<uint32_t>(sizeof uint32_t), li::BufferUsage::DynamicDraw);

	m_VertexArray->AddVertexBuffer(m_VertexBuffer);
	m_VertexArray->SetIndexBuffer(m_IndexBuffer);
}

void DebugPhysicsRenderer::Render(li::OrthographicCamera* camera, float z)
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
				m_Vertices[m_VertexCount].Position = { command.Point1.x - 0.01, command.Point1.y };
				m_Vertices[m_VertexCount].Color = command.Color;
				m_Indices[m_IndexCount] = m_VertexCount;
				m_VertexCount++;
				m_IndexCount++;

				m_Vertices[m_VertexCount].Position = { command.Point1.x + 0.01, command.Point1.y };
				m_Vertices[m_VertexCount].Color = command.Color;
				m_Indices[m_IndexCount] = m_VertexCount;
				m_VertexCount++;
				m_IndexCount++;

				m_Vertices[m_VertexCount].Position = { command.Point1.x, command.Point1.y - 0.01 };
				m_Vertices[m_VertexCount].Color = command.Color;
				m_Indices[m_IndexCount] = m_VertexCount;
				m_VertexCount++;
				m_IndexCount++;

				m_Vertices[m_VertexCount].Position = { command.Point1.x, command.Point1.y + 0.01 };
				m_Vertices[m_VertexCount].Color = command.Color;
				m_Indices[m_IndexCount] = m_VertexCount;
				m_VertexCount++;
				m_IndexCount++;

				break;
			case DebugDrawType::Line:
				// Draw a line with the two vertices.
				m_Vertices[m_VertexCount].Position = command.Point1;
				m_Vertices[m_VertexCount].Color = command.Color;
				m_Indices[m_IndexCount] = m_VertexCount;
				m_VertexCount++;
				m_IndexCount++;

				m_Vertices[m_VertexCount].Position = command.Point2;
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
					m_Vertices[m_VertexCount].Position = command.Vertices[i];
					m_Vertices[m_VertexCount].Color = command.Color;

					m_Indices[m_IndexCount++] = m_VertexCount;
					m_Indices[m_IndexCount++] = (i == command.VertexCount - 1) ? firstIndex : m_VertexCount + 1;

					m_VertexCount++;
				}

				delete[] command.Vertices;
				break;
			}
			case DebugDrawType::Circle:
			{
				uint32_t firstIndex = m_VertexCount;

				const int resolution = 32;
				for (int i = 0; i < resolution; i++)
				{
					float angle = (float)i / (float)resolution * 2.0f * (float)M_PI;
					
					m_Vertices[m_VertexCount].Position = { 
						SDL_cosf(angle) * command.Radius + command.Point1.x, 
						SDL_sinf(angle) * command.Radius + command.Point1.y };
					m_Vertices[m_VertexCount].Color = command.Color;

					m_Indices[m_IndexCount++] = m_VertexCount;
					m_Indices[m_IndexCount++] = (i == resolution - 1) ? firstIndex : m_VertexCount + 1;

					m_VertexCount++;
				}

				break;
			}
			}

			m_CommandQueue.pop();
		}

		m_VertexBuffer->SetSubData((float*)m_Vertices.data(), m_VertexCount * sizeof(DebugPhysicsVertex), 0);
		m_IndexBuffer->SetSubData(m_Indices.data(), m_IndexCount * sizeof(uint32_t), 0);
	}

	// Render the lines.
	if (m_IndexCount > 0)
	{
		m_VertexArray->Bind();
		li::Ref<li::Shader> shader = li::ResourceManager::Get<li::Shader>("shader_debug_physics");
		shader->Bind();
		shader->SetFloat("u_ZPosition", z);
		shader->SetMat4("u_ViewProjection", camera->GetViewProjectionMatrix());
		
		li::RendererAPI::DrawIndexed(m_VertexArray, m_IndexCount, li::DrawMode::Lines);
		m_VertexArray->Unbind();
	}
}

#endif