#include "pch.h"
#include "TickThread.h"

#include "Game.h"

int TickThreadEntryPoint(const TickThreadData& data)
{
	LI_INFO("Tick thread starting...");

	Game game = Game(data);
	game.Run();

	LI_INFO("Tick thread closing...");

	return 0;
}

#ifdef HZ_PHYSICS_DEBUG_DRAW
int TickThreadEntryPointDebugDraw(const TickThreadData& data, DebugDrawCommandQueue* debugDrawQueue)
{
	LI_INFO("Tick thread starting...");

	Game game = Game(data, debugDrawQueue);
	game.Run();

	LI_INFO("Tick thread closing...");

	return 0;
}
#endif