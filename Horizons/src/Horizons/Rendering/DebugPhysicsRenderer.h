#pragma once

#include "Horizons/Core/Core.h"

#ifdef HZ_PHYSICS_DEBUG_DRAW
#include "DebugPhysicsDrawShared.h"
#include "Lithium.h"

#include <queue>

class DebugPhysicsRenderer
{
public:
	void SubmitLines(DebugDrawCommandQueue* queue);

private:
	constexpr static float ZPos = 0.0f;

	std::deque<DebugDrawCommand> m_FeedQueue;
	std::deque<DebugDrawCommand> m_CurrentQueue;
};

#endif