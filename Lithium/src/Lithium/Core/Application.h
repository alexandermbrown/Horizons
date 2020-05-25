#pragma once

#include "Lithium/Core/Window.h"
#include "Lithium/Core/LayerStack.h"
#include "Lithium/ImGui/ImGuiRenderer.h"

#include <functional>

namespace li
{
	class Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();

		void OnEvent(SDL_Event* event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		

		inline static Application& Get() { return *s_Instance; }
		inline Ref<Window>& GetWindow() { return m_Window;  }

		inline const std::function<void(SDL_Event* event)>& GetEventCallbackFn() { return m_EventCallbackFn; }

	private:
		void OnWindowEvent(SDL_Event* event);

		bool m_Running;
		unsigned int m_LastTicks;
		Ref<Window> m_Window;
		Scope<ImGuiRenderer> m_ImGuiRenderer;
		LayerStack m_LayerStack;
		std::function<void(SDL_Event* event)> m_EventCallbackFn;
		
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}