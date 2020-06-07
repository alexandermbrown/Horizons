#include "pch.h"
#include "DebugDrawSystem.h"

#ifdef HZ_PHYSICS_DEBUG_DRAW
#include "PhysicsComponents.h"

namespace DebugDrawSystem
{
	void Init(entt::registry& registry, DebugDrawCommandQueue* queue)
	{
		// Get physics world from the registry.
		auto worldView = registry.view<cp::physics_world>();
		entt::entity worldEnt = worldView.front();
		cp::physics_world& world = worldView.get(worldEnt);

		cp::physics_debug_draw& draw = registry.emplace<cp::physics_debug_draw>(worldEnt); 
		draw.draw = new PhysicsDebugDraw(queue); // TODO: parse in reader writer queue pointer.
		draw.draw->SetFlags(0b01011);
		world.world->SetDebugDraw(draw.draw);
	}

	void Draw(entt::registry& registry)
	{
		// Get physics world from the registry.
		auto worldView = registry.group<cp::physics_world, cp::physics_debug_draw>();
		entt::entity entity = worldView.front();

		cp::physics_world& world = worldView.get<cp::physics_world>(entity);
		cp::physics_debug_draw debugDraw = worldView.get<cp::physics_debug_draw>(entity);

		world.world->DebugDraw();
		debugDraw.draw->EndDraw();
	}

	void Shutdown(entt::registry& registry)
	{
		// Get physics debug draw from the registry.
		auto drawView = registry.view<cp::physics_debug_draw>();
		cp::physics_debug_draw& debugDraw = drawView.get(drawView.front());

		delete debugDraw.draw;
	}
}

PhysicsDebugDraw::PhysicsDebugDraw(DebugDrawCommandQueue* queue)
	: b2Draw(), m_CommandQueue(queue) {}

void PhysicsDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	DebugDrawCommand command;
	command.Type = DebugDrawType::PolygonOutline;
	command.Color = { color.r, color.g, color.b, color.a };

	command.Vertices = new glm::vec2[vertexCount];
	command.VertexCount = vertexCount;

	for (int i = 0; i < vertexCount; i++)
	{
		command.Vertices[i] = { vertices[i].x, vertices[i].y };
	}

	m_CommandQueue->enqueue(command);
}

void PhysicsDebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
	DebugDrawCommand command;
	command.Type = DebugDrawType::Circle;
	command.Color = { color.r, color.g, color.b, color.a };

	command.Point1 = { center.x, center.y };
	command.Radius = radius;

	m_CommandQueue->enqueue(command);
}

void PhysicsDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	DebugDrawCommand command;
	command.Type = DebugDrawType::Line;
	command.Color = { color.r, color.g, color.b, color.a };

	command.Point1 = { p1.x, p1.y };
	command.Point2 = { p2.x, p2.y };

	m_CommandQueue->enqueue(command);
}

void PhysicsDebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
	DebugDrawCommand command;
	command.Type = DebugDrawType::Point;
	command.Color = { color.r, color.g, color.b, color.a };

	command.Point1 = { p.x, p.y };
	command.Radius = size;

	m_CommandQueue->enqueue(command);
}

void PhysicsDebugDraw::EndDraw()
{
	DebugDrawCommand command;
	command.Type = DebugDrawType::End;
	m_CommandQueue->enqueue(command);
}

#endif