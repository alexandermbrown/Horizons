#pragma once

#include "Lithium/Core/Window.h"
#include "Lithium/Core/LayerStack.h"
#include "Lithium/Events/Event.h"
#include "Lithium/Events/WindowEvent.h"
#include "Lithium/ImGui/ImGuiRenderer.h"

namespace li
{
	class Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		

		inline static Application& Get() { return *s_Instance; }
		inline Ref<Window>& GetWindow() { return m_Window;  }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		bool m_Running;
		unsigned int m_LastTicks;
		Ref<Window> m_Window;
		Scope<ImGuiRenderer> m_ImGuiRenderer;
		LayerStack m_LayerStack;
		
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}