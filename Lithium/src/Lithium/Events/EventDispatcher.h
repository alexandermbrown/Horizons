#pragma once

#include "SDL.h"
#include <stdint.h>

namespace li
{
	class EventDispatcher
	{
	public:
		EventDispatcher(SDL_Event* event)
			: m_Event(event) {}

		template<typename F>
		bool Dispatch(uint32_t sdlEventType, const F& func)
		{
			if (m_Event->type == sdlEventType)
			{
				func(m_Event);
				return true;
			}
			return false;
		}

	private:
		SDL_Event* m_Event;
	};
}