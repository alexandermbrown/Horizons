#pragma once

#include "Lithium/Core/Core.h"

#include <glm/glm.hpp>
#include <SDL.h>

#define LI_SDL_SCANCODE_COUNT 284

namespace li
{
	class Input
	{
	public:

		Input();

		void OnEvent(SDL_Event* event);

		inline bool IsKeyPressed(int scancode) const 
		{
			if (m_Enabled)
				return m_KeyStates[scancode];
			else
				return false;
		}
		inline bool IsMouseButtonPressed(int sdl_button) const
		{ 
			if (m_Enabled)
				return m_Mouse.ButtonStates & SDL_BUTTON(sdl_button);
			else
				return false;
		}

		inline int GetMouseX() const { return m_Mouse.X; }
		inline int GetMouseY() const { return m_Mouse.Y; }

		inline bool IsEnabled() const { return m_Enabled; }
		inline void Enable() { m_Enabled = true; }
		inline void Disable() { m_Enabled = false; }

	private:

		struct MouseState
		{
			int X, Y;
			uint8_t ButtonStates;
		};

		bool m_KeyStates[LI_SDL_SCANCODE_COUNT];

		MouseState m_Mouse;

		bool m_Enabled;
	};
}