#pragma once

#include "Lithium/Core/Layer.h"

#include "Lithium/Events/Event.h"
#include "Lithium/Events/ApplicationEvent.h"
#include "Lithium/Events/KeyEvent.h"
#include "Lithium/Events/MouseEvent.h"

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
		void OnSDLEvent(SDL_Event* event);

		bool WantCapture(const SDL_Event& event);
	private:
		float m_Time = 0.0f;
	};

}