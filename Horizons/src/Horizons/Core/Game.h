#pragma once

#include "Lithium.h"
#include "readerwriterqueue/readerwriterqueue.h"
#include "entt/entt.hpp"

class Game
{
public:

	Game(moodycamel::ReaderWriterQueue<SDL_Event>* eventQueue);
	void Run();

	void OnEvent(SDL_Event* event);


private:

	bool m_Running;
	std::chrono::time_point<std::chrono::steady_clock> m_LastUpdateTime;
	moodycamel::ReaderWriterQueue<SDL_Event>* m_EventQueue;

	entt::registry m_Registry;
};