#include "pch.h"
#include "TickThread.h"

#include "Game.h"

int TickThreadEntryPoint(moodycamel::ReaderWriterQueue<SDL_Event>* eventQueue, SyncEventQueue* syncQueue, SyncTransformQueue* transformQueue, const ConfigStore& config)
{
	LI_INFO("Tick thread starting...");

	Game game = Game(eventQueue, syncQueue, transformQueue, config);
	game.Run();

	LI_INFO("Tick thread closing...");

	return 0;
}

#ifdef HZ_PHYSICS_DEBUG_DRAW
int TickThreadEntryPointDebugDraw(moodycamel::ReaderWriterQueue<SDL_Event>* eventQueue, SyncEventQueue* syncQueue, SyncTransformQueue* transformQueue, const ConfigStore& config, DebugDrawCommandQueue* debugDrawQueue)
{
	LI_INFO("Tick thread starting...");

	Game game = Game(eventQueue, syncQueue, transformQueue, config, debugDrawQueue);
	game.Run();

	LI_INFO("Tick thread closing...");

	return 0;
}
#endif