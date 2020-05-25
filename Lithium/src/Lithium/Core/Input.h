#pragma once

#include "Lithium/Core/Core.h"

#include <glm/glm.hpp>

namespace li
{
	class Input
	{
	public:

		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static glm::ivec2 GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static int GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static int GetMouseY() { return s_Instance->GetMouseYImpl(); }

		inline static glm::ivec2 GetMousePos() { return s_Instance->GetMousePos(); }

	protected:

		bool IsKeyPressedImpl(int keycode);

		bool IsMouseButtonPressedImpl(int button);
		glm::ivec2 GetMousePositionImpl();
		int GetMouseXImpl();
		int GetMouseYImpl();

	private:
		static Scope<Input> s_Instance;
	};
}