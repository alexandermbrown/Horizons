#include "pch.h"
#include "DebugPhysicsRenderer.h"
#ifdef HZ_PHYSICS_DEBUG_DRAW

#include <math.h>

void DebugPhysicsRenderer::SubmitLines(DebugDrawCommandQueue* queue)
{
	DebugDrawCommand command;
	while (queue->try_dequeue(command)) // TODO: check if there are multiple ENDs in the queue, only render most recent.
	{
		if (command.Type == DebugDrawType::End)
		{
			m_FeedQueue.swap(m_CurrentQueue);
			m_FeedQueue.clear();
		}
		else
		{
			m_FeedQueue.push_back(command);
		}
	}

	for (auto& command : m_CurrentQueue)
	{
		switch (command.Type)
		{
		case DebugDrawType::Point:
			// Draw a '+' with two lines to represent the point.
			Li::Renderer::SubmitLine(command.Color,
				{ command.Point1.x - 0.01, command.Point1.y, ZPos },
				{ command.Point1.x + 0.01, command.Point1.y, ZPos });

			Li::Renderer::SubmitLine(command.Color,
				{ command.Point1.x, command.Point1.y - 0.01, ZPos },
				{ command.Point1.x, command.Point1.y + 0.01, ZPos });
			break;
		case DebugDrawType::Line:
			Li::Renderer::SubmitLine(command.Color,
				{ command.Point1, ZPos },
				{ command.Point2, ZPos });
			break;
		case DebugDrawType::PolygonOutline:
		{
			LI_ASSERT(command.VertexCount > 1, "Polygons must have more than 1 vertice!");

			for (uint32_t i = 0; i < command.VertexCount; i++)
			{
				glm::vec3 point2;
				if (i == command.VertexCount - 1)
					point2 = { command.Vertices[0], ZPos };
				else
					point2 = { command.Vertices[i + 1], ZPos };

				Li::Renderer::SubmitLine(command.Color, { command.Vertices[i], ZPos }, point2);
			}
			delete[] command.Vertices;
			break;
		}
		case DebugDrawType::Circle:
		{
			Li::Renderer::SubmitCircle(command.Color, { command.Point1, ZPos }, command.Radius);
			break;
		}
		}
	}
}
#endif
