#include "pch.h"
#include "TickThread.h"

#include "Game.h"

int TickThreadEntryPoint(moodycamel::ReaderWriterQueue<SDL_Event>* eventQueue)
{
	LI_TRACE("Tick thread starting...");

	Game game = Game(eventQueue);
	game.Run();

	return 0;
}

#ifdef HZ_PHYSICS_DEBUG_DRAW
int TickThreadEntryPointDebugDraw(moodycamel::ReaderWriterQueue<SDL_Event>* eventQueue, DebugDrawCommandQueue* debugDrawQueue)
{
	LI_TRACE("Tick thread starting...");

	Game game = Game(eventQueue, debugDrawQueue);
	game.Run();

	return 0;
}
#endif