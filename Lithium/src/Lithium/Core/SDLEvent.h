#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Events/Event.h"

#include <SDL.h>
#include <functional>

namespace li
{
	namespace SDLEvent
	{
		using EventCallbackFn = std::function<void(Event&)>;
		void Broadcast(SDL_Event& sdlEvent, const EventCallbackFn& callback);
	}
}