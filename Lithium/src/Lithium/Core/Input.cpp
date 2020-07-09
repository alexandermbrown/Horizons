#include "lipch.h"
#include "Input.h"

namespace li
{
	Input::Input()
		: m_KeyStates(), m_Mouse({ 0, 0, 0 }), m_Enabled(true)
	{
	}

	void Input::OnEvent(SDL_Event* event)
	{
		// Keyboard events.
		if (event->type == SDL_KEYDOWN)
		{
			m_KeyStates[event->key.keysym.scancode] = true;
		}
		else if (event->type == SDL_KEYUP)
		{
			m_KeyStates[event->key.keysym.scancode] = false;
		}
		
		// Mouse button events.
		else if (event->type == SDL_MOUSEBUTTONDOWN)
		{
			m_Mouse.ButtonStates |= SDL_BUTTON(event->button.button);
		}
		else if (event->type == SDL_MOUSEBUTTONUP)
		{
			m_Mouse.ButtonStates &= ~SDL_BUTTON(event->button.button);
		}

		// Mouse motion event.
		else if (event->type == SDL_MOUSEMOTION)
		{
			m_Mouse.X = event->motion.x;
			m_Mouse.X = event->motion.y;
		}
	}
}