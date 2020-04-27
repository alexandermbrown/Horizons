#include "lipch.h"
#include "Input.h"

#include <SDL.h>

namespace li
{
	Scope<Input> Input::s_Instance = CreateScope<Input>();

	Input::Input()
	{
	}

	Input::~Input()
	{
	}

	bool Input::IsKeyPressedImpl(int keycode)
	{
		SDL_Scancode scancode = SDL_GetScancodeFromKey(keycode);
		SDL_PumpEvents();
		const uint8_t* state = SDL_GetKeyboardState(nullptr);

		return state[scancode];
	}

	bool Input::IsMouseButtonPressedImpl(int button)
	{
		SDL_PumpEvents();
		const uint8_t state = SDL_GetMouseState(nullptr, nullptr);

		return state & SDL_BUTTON(button);
	}

	glm::ivec2 Input::GetMousePositionImpl()
	{
		int x, y;
		SDL_PumpEvents();
		SDL_GetMouseState(&x, &y);

		return glm::ivec2(x, y);
	}

	int Input::GetMouseXImpl()
	{
		return GetMousePositionImpl().x;
	}

	int Input::GetMouseYImpl()
	{
		return GetMousePositionImpl().y;
	}
}