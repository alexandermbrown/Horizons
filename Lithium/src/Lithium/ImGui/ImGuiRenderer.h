#pragma once

#include "Lithium/Core/Layer.h"

#include "SDL.h"

namespace li 
{
	class ImGuiRenderer
	{
	public:
		ImGuiRenderer();
		virtual ~ImGuiRenderer();

		void Begin();
		void End();
		void OnEvent(SDL_Event* event);
		void Resize(int width, int height);

		bool WantCapture(const SDL_Event& event);
	private:
		float m_Time = 0.0f;
	};

}