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
