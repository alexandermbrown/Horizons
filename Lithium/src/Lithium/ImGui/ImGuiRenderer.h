#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Core/Layer.h"

#include "SDL.h"

namespace li 
{
	class ImGuiRenderer
	{
	public:
		virtual ~ImGuiRenderer() = default;

		virtual void Begin() = 0;
		virtual void End() = 0;
		void OnEvent(SDL_Event* event);
		void Resize(int width, int height);

		static Ref<ImGuiRenderer> Create();
	};
}