#pragma once

#include "glm/glm.hpp"
#include "SDL.h"

namespace Li
{
	class Input
	{
	public:
		Input();

		void OnEvent(SDL_Event* event);

		bool IsKeyPressed(int scancode) const;
		bool IsMouseButtonPressed(int sdl_button) const;

		inline int GetMouseX() const { return m_MouseX; }
		inline int GetMouseY() const { return m_MouseY; }

		inline bool IsEnabled() const { return m_Enabled; }
		inline void Enable() { m_Enabled = true; }
		inline void Disable() { m_Enabled = false; }

	private:
		constexpr static int ScancodeCount = 285;

		bool m_Enabled;

		int m_MouseX;
		int m_MouseY;
		uint8_t m_MouseButtonStates;

		bool m_KeyStates[ScancodeCount];
	};
}
