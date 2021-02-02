#pragma once

#include "Lithium/Core/Memory.h"
#include "Lithium/Core/Window.h"
#include "Lithium/Core/LayerStack.h"
#include "Lithium/Core/Input.h"
#include "Lithium/Core/Scene.h"

#include "Lithium/Renderer/RendererAPI.h"

#ifndef LI_DIST
#include "Lithium/ImGui/ImGuiRenderer.h"
#endif

#include <functional>
#include <chrono>
#include <string>

namespace Li
{
	class Application
	{
	public:
		using EventCallbackFn = std::function<void(SDL_Event* event)>;

		inline static Application& Get() { return *s_Instance; }

		template<typename T>
		inline static T& Get()
		{
			static_assert(std::is_base_of<Application, T>::value, "T is not a subclass of Application!");
			return *static_cast<T*>(s_Instance);
		}

		Application(const WindowProps& props);
		virtual ~Application();
		void Run();
		inline void Exit() { m_Running = false; }

		void OnEvent(SDL_Event* event);
		inline void EventHandled() { m_EventHandled = true; }
		inline void TakeFocus(const std::string& layer) { m_FocusedLayer = layer; }
		inline void ReleaseFocus() { m_FocusedLayer = ""; }

		inline void PushLayer(Unique<Layer> layer) { m_LayerStack.PushLayer(std::move(layer)); }
		inline void PushOverlay(Unique<Layer> layer) { m_LayerStack.PushOverlay(std::move(layer)); }
		inline void PopLayer(const std::string_view& name) { m_LayerStack.PopLayer(name); }
		inline void PopOverlay(const std::string_view& name) { m_LayerStack.PopOverlay(name); }

		void Transition(Unique<Scene> next_scene, bool instant);
		inline void FinishTransition() { m_TransitionFinished = true; }

		inline Window& GetWindow() { return *m_Window; }
		inline const EventCallbackFn& GetEventCallbackFn() { return m_EventCallbackFn; }
		inline const Input& GetInput() { return m_Input; }
		inline RendererAPI GetAPI() const { return m_RendererAPI; }
#ifndef LI_DIST
		inline const Unique<ImGuiRenderer>& GetImGuiRenderer() { return m_ImGuiRenderer; }
#endif

	protected:
		inline void ClearScene() {
			m_CurrentScene.reset();
			m_NextScene.reset();
		}
		inline void ClearLayers() { m_LayerStack.Clear(); }

	private:
		void OnWindowEvent(SDL_Event* event);

		bool m_Running;
		std::chrono::time_point<std::chrono::steady_clock> m_LastUpdateTime;

		RendererAPI m_RendererAPI;
		Unique<Window> m_Window;
		Input m_Input;
#ifndef LI_DIST
		Unique<ImGuiRenderer> m_ImGuiRenderer;
#endif
		EventCallbackFn m_EventCallbackFn;
		bool m_EventHandled;

		LayerStack m_LayerStack;
		std::string m_FocusedLayer;
		
		Unique<Scene> m_CurrentScene;
		Unique<Scene> m_NextScene;
		bool m_CallOnTransition;
		bool m_TransitionFinished;

		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Unique<Application> CreateApplication();
}
