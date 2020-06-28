#pragma once

#include "Lithium/Core/Window.h"
#include "Lithium/Core/LayerStack.h"
#include "Lithium/ImGui/ImGuiRenderer.h"
#include "Lithium/Core/Input.h"

#include <functional>

namespace li
{
	struct WindowProps
	{
		char* Title;
		int Width;
		int Height;
		bool Resizable;
		bool Shown;
		bool Borderless;
	};

	class Application
	{
	public:
		inline static Application* Get() { return s_Instance; }

		template<typename T>
		inline static T* Get() { return static_cast<T*>(s_Instance); }

		Application(const WindowProps& props);
		virtual ~Application();
		void Run();

		void OnEvent(SDL_Event* event);
		inline void EventHandled() { m_EventHandled = true; }
		inline void TakeFocus(Layer* layer) { m_FocusedLayer = layer; }

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		inline Ref<Window>& GetWindow() { return m_Window;  }
		inline const std::function<void(SDL_Event* event)>& GetEventCallbackFn() { return m_EventCallbackFn; }
		inline const Input& GetInput() { return m_Input; }

	private:
		void OnWindowEvent(SDL_Event* event);

		bool m_Running;
		unsigned int m_LastTicks;

		Ref<Window> m_Window;
		Scope<ImGuiRenderer> m_ImGuiRenderer;
		LayerStack m_LayerStack;
		Input m_Input;

		std::function<void(SDL_Event* event)> m_EventCallbackFn;
		
		static Application* s_Instance;

		bool m_EventHandled;
		bool m_LayersDirty;

		Layer* m_FocusedLayer;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}