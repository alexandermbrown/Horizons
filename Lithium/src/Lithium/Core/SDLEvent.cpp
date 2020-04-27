#include "lipch.h"
#include "SDLEvent.h"

#include "Lithium/Events/WindowEvent.h"
#include "Lithium/Events/MouseEvent.h"
#include "Lithium/Events/KeyEvent.h"

namespace li
{
	namespace SDLEvent
	{
		void Broadcast(SDL_Event& sdlEvent, const EventCallbackFn& callback)
		{
			if (sdlEvent.type == SDL_WINDOWEVENT)
			{
				switch (sdlEvent.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
				{
					WindowResizeEvent ev(sdlEvent.window.windowID, sdlEvent.window.data1, sdlEvent.window.data2);
					callback(ev);
					break;
				}
				case SDL_WINDOWEVENT_MOVED:
				{
					WindowMovedEvent ev(sdlEvent.window.windowID, sdlEvent.window.data1, sdlEvent.window.data2);
					callback(ev);
					break;
				}
				case SDL_WINDOWEVENT_MINIMIZED:
				{
					WindowMinimizedEvent ev(sdlEvent.window.windowID);
					callback(ev);
					break;
				}
				case SDL_WINDOWEVENT_MAXIMIZED:
				{
					WindowMaximizedEvent ev(sdlEvent.window.windowID);
					callback(ev);
					break;
				}
				case SDL_WINDOWEVENT_RESTORED:
				{
					WindowRestoredEvent ev(sdlEvent.window.windowID);
					callback(ev);
					break;
				}
				case SDL_WINDOWEVENT_FOCUS_GAINED:
				{
					WindowFocusGainedEvent ev(sdlEvent.window.windowID);
					callback(ev);
					break;
				}
				case SDL_WINDOWEVENT_FOCUS_LOST:
				{
					WindowFocusLostEvent ev(sdlEvent.window.windowID);
					callback(ev);
					break;
				}
				case SDL_WINDOWEVENT_CLOSE:
				{
					WindowCloseEvent ev(sdlEvent.window.windowID);
					callback(ev);
					break;
				}
				}
			}
			else if (sdlEvent.type == SDL_KEYDOWN)
			{
				KeyPressedEvent ev(sdlEvent.key.keysym.sym, sdlEvent.key.repeat);
				callback(ev);
			}
			else if (sdlEvent.type == SDL_KEYUP)
			{
				KeyReleasedEvent ev(sdlEvent.key.keysym.sym);
				callback(ev);
			}
			else if (sdlEvent.type == SDL_TEXTINPUT)
			{
				TextInputEvent ev(sdlEvent.text.text);
				callback(ev);
			}
			else if (sdlEvent.type == SDL_MOUSEMOTION)
			{
				MouseMovedEvent ev(sdlEvent.motion.x, sdlEvent.motion.y);
				callback(ev);
			}
			else if (sdlEvent.type == SDL_MOUSEBUTTONDOWN)
			{
				MouseButtonPressedEvent ev(sdlEvent.button.button);
				callback(ev);
			}
			else if (sdlEvent.type == SDL_MOUSEBUTTONUP)
			{
				MouseButtonReleasedEvent ev(sdlEvent.button.button);
				callback(ev);
			}
			else if (sdlEvent.type == SDL_MOUSEWHEEL)
			{
				MouseScrolledEvent ev(sdlEvent.wheel.x, sdlEvent.wheel.y);
				callback(ev);
			}
		}
	}
}


