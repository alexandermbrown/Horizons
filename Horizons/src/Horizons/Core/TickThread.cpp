#include "pch.h"
#include "TickThread.h"

#include "Game.h"

int TickThreadEntryPoint(moodycamel::ReaderWriterQueue<SDL_Event>* eventQueue)
{
	LI_INFO("Tick thread starting...");

	Game game = Game(eventQueue);
	game.Run();

	LI_INFO("Tick thread closing...");

	return 0;
}

#ifdef HZ_PHYSICS_DEBUG_DRAW
int TickThreadEntryPointDebugDraw(moodycamel::ReaderWriterQueue<SDL_Event>* eventQueue, DebugDrawCommandQueue* debugDrawQueue)
{
	LI_INFO("Tick thread starting...");

	Game game = Game(eventQueue, debugDrawQueue);
	game.Run();

	LI_INFO("Tick thread closing...");

	return 0;
}
#endif