#pragma once

#include "Lithium/Core/Window.h"
#include "Lithium/Core/LayerStack.h"
#include "Lithium/Core/Input.h"
#include "Lithium/Core/Scene.h"

#include "Lithium/Renderer/RendererAPI.h"

#include "Lithium/ImGui/ImGuiRenderer.h"

#include <functional>
#include <chrono>

namespace li
{
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

		inline Window* GetWindow() { return m_Window;  }
		inline const std::function<void(SDL_Event* event)>& GetEventCallbackFn() { return m_EventCallbackFn; }
		inline const Input& GetInput() { return m_Input; }
		inline RendererAPI GetAPI() const { return m_RendererAPI; }

		void Transition(Scene* scene);

	private:
		void OnWindowEvent(SDL_Event* event);

		RendererAPI m_RendererAPI;

		bool m_Running;
		std::chrono::time_point<std::chrono::steady_clock> m_LastUpdateTime;

		Window* m_Window;
		Ref<ImGuiRenderer> m_ImGuiRenderer;
		LayerStack m_LayerStack;
		Input m_Input;

		std::function<void(SDL_Event* event)> m_EventCallbackFn;

		bool m_EventHandled;
		bool m_LayersDirty;

		Layer* m_FocusedLayer;
		
		Scene* m_CurrentScene;
		Scene* m_NextScene;

		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}