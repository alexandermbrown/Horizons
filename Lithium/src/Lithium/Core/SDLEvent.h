#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Events/Event.h"

#include <SDL.h>
#include <functional>

namespace li
{
	namespace SDLEvent
	{
		void Broadcast(SDL_Event& sdlEvent, const std::function<void(Event&)>& callback);
	}
}