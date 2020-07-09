#pragma once

#include "Horizons/Core/Core.h"

#ifdef HZ_PHYSICS_DEBUG_DRAW
#include "Horizons/Rendering/DebugPhysicsDrawShared.h"

#include "box2d/box2d.h"
#include "entt/entt.hpp"
#include "readerwriterqueue/readerwriterqueue.h"
#include "glm/glm.hpp"

namespace DebugDrawSystem
{
	void Init(entt::registry& registry, DebugDrawCommandQueue* queue);
	void Draw(entt::registry& registry);
	void Shutdown(entt::registry& registry);
}

class PhysicsDebugDraw : public b2Draw
{
public:
	PhysicsDebugDraw(DebugDrawCommandQueue* queue);

	// Draw a closed polygon provided in CCW order.
	virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

	// Draw a solid closed polygon provided in CCW order.
	virtual inline void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
	{
		DrawPolygon(vertices, vertexCount, color);
	}

	// Draw a circle.
	virtual void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;

	// Draw a solid circle.
	virtual inline void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override
	{
		DrawSegment(center, center + b2Vec2(axis.x * radius, axis.y * radius), color);
		DrawCircle(center, radius, color);
	}

	// Draw a line segment.
	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

	// Draw a transform. Choose your own length scale.
	virtual inline void DrawTransform(const b2Transform& xf) override {};

	// Draw a point.
	virtual void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;

	// End the current draw.
	void EndDraw();

private:
	DebugDrawCommandQueue* m_CommandQueue;
};

#endif