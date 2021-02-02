#include "lipch.h"
#include "Input.h"

#include "Lithium/Core/Assert.h"

namespace Li
{
	Input::Input()
		: m_Enabled(true), m_MouseX(0), m_MouseY(0), m_MouseButtonStates(0), m_KeyStates()
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
			m_MouseButtonStates |= SDL_BUTTON(event->button.button);
		}
		else if (event->type == SDL_MOUSEBUTTONUP)
		{
			m_MouseButtonStates &= ~SDL_BUTTON(event->button.button);
		}

		// Mouse motion event.
		else if (event->type == SDL_MOUSEMOTION)
		{
			m_MouseX = event->motion.x;
			m_MouseX = event->motion.y;
		}
	}

	bool Input::IsKeyPressed(int scancode) const
	{
		LI_CORE_ASSERT(scancode >= 0 && scancode < ScancodeCount, "Invalid scancode.");
		if (m_Enabled)
			return m_KeyStates[scancode];
		else
			return false;
	}

	bool Input::IsMouseButtonPressed(int sdl_button) const
	{
		LI_CORE_ASSERT(sdl_button >= 0 && sdl_button < 8, "Invalid mouse button.");
		if (m_Enabled)
			return m_MouseButtonStates & SDL_BUTTON(sdl_button);
		else
			return false;
	}
}
