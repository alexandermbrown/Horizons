#include "lipch.h"
#include "Application.h"

#include "Lithium/Core/Core.h"
#include "Lithium/Core/Log.h"
#include "Lithium/Renderer/Renderer.h"
#include "Lithium/Resources/ResourceManager.h"
#include "Lithium/Audio/AudioManager.h"
#include "Lithium/Localization/Localization.h"
#include "Lithium/UI/UI.h"

#include "SDL.h"
#ifndef LI_DIST
#include "imgui.h"
#endif

namespace li 
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const WindowProps& props)
		: m_Running(false), m_LayerStack(), m_EventHandled(false), m_Window(nullptr), m_RendererAPI(props.API),
		m_Input(), m_FocusedLayer(nullptr), m_LayersDirty(false), m_CurrentScene(nullptr), m_NextScene(nullptr)
	{
		LI_CORE_ASSERT(s_Instance == nullptr, "Instance of Application already exists!");
		s_Instance = this;

		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			LI_CORE_ERROR("Failed to initialize SDL!");
			return;
		}

		SDL_version compiledVersion;

		SDL_VERSION(&compiledVersion);
		LI_CORE_INFO("SDL compiled ver {0}.{1}.{2}", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);

#ifdef LI_DEBUG
		SDL_version linkedVersion;
		SDL_GetVersion(&linkedVersion);
		LI_CORE_INFO("SDL linked ver {0}.{1}.{2}", linkedVersion.major, linkedVersion.minor, linkedVersion.patch);

		LI_CORE_ASSERT(
			linkedVersion.major == SDL_MAJOR_VERSION &&
			linkedVersion.minor == SDL_MINOR_VERSION &&
			linkedVersion.patch == SDL_PATCHLEVEL,
			"SDL compiled and linked to two different versions!"
		);
#endif

		m_EventCallbackFn = LI_BIND_FN(Application::OnEvent);

		m_Window = Window::Create(props);
		m_Window->GetContext()->ResizeView(props.Width, props.Height);
#ifndef LI_DIST
		m_ImGuiRenderer = ImGuiRenderer::Create();
#endif
		AudioManager::Init(nullptr);
	}

	Application::~Application()
	{
		ResourceManager::Shutdown();
		UI::Shutdown();
		AudioManager::Shutdown();
		Renderer::Shutdown();

		delete m_Window;

		SDL_Quit();
	}

	void Application::Run()
	{
		m_Running = true;
		m_LastUpdateTime = std::chrono::steady_clock::now();

		while (m_Running)
		{
			//////////////////////
			// Propagate Events //
			//////////////////////
			SDL_Event sdlEvent;
			while (SDL_PollEvent(&sdlEvent))
			{
				OnEvent(&sdlEvent);
			}

			//////////////////////////
			// Calculate Delta Time //
			//////////////////////////

			std::chrono::time_point<std::chrono::steady_clock> currentTime = std::chrono::steady_clock::now();
			duration::us dt = duration::cast<duration::us>(currentTime - m_LastUpdateTime);
			m_LastUpdateTime = currentTime;


			//////////////////
			// Update Scene //
			//////////////////

			if (m_CurrentScene)
			{
				m_CurrentScene->OnUpdate(dt);

				if (m_NextScene && m_CurrentScene->Finished())
				{
					delete m_CurrentScene;
					m_CurrentScene = m_NextScene;
					m_CurrentScene->TransitionIn();
					m_NextScene = nullptr;
				}
			}


			///////////////////
			// Update Layers //
			///////////////////

			bool reachedFocused = false;
			if (m_FocusedLayer)
				m_Input.Disable();

			m_LayersDirty = false;

			for (Layer* layer : m_LayerStack)
			{
				if (m_FocusedLayer == layer && !reachedFocused)
				{
					m_Input.Enable();
					reachedFocused = true;
				}

				layer->OnUpdate(dt);

				if (m_LayersDirty)
					break;
			}


			//////////////////
			// Render ImGui //
			//////////////////

			reachedFocused = false;
			if (m_FocusedLayer)
				m_Input.Disable();

#ifndef LI_DIST
			m_ImGuiRenderer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				if (m_FocusedLayer == layer && !reachedFocused)
				{
					m_Input.Enable();
					reachedFocused = true;
				}
				layer->OnImGuiRender();

				if (m_LayersDirty)
					break;
			}
			m_ImGuiRenderer->End();
#endif
			m_Window->SwapBuffers();
		}

		delete m_CurrentScene;
	}

	void Application::OnEvent(SDL_Event* event)
	{
		m_Input.OnEvent(event);

		if (event->type == SDL_WINDOWEVENT)
			OnWindowEvent(event);

#ifndef LI_DIST
		m_ImGuiRenderer->OnEvent(event);
#endif
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (m_EventHandled)
				break;
			(*it)->OnEvent(event);
		}

		if (!m_EventHandled && event->window.event == SDL_WINDOWEVENT_CLOSE)
			m_Running = false;

		m_EventHandled = false;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayersDirty = true;
		m_LayerStack.PopLayer(layer);
	}

	void Application::PopOverlay(Layer* overlay)
	{
		m_LayersDirty = true;
		m_LayerStack.PopOverlay(overlay);
	}

	void Application::Transition(Scene* nextScene)
	{
		m_NextScene = nullptr;
		if (m_CurrentScene)
		{
			m_CurrentScene->TransitionOut();
			if (m_CurrentScene->Finished())
			{
				delete m_CurrentScene;
				m_CurrentScene = nextScene;
				m_CurrentScene->TransitionIn();
			}
			else
			{
				m_NextScene = nextScene;
			}
		}
		else
		{
			m_CurrentScene = nextScene;
			nextScene->TransitionIn();
		}
	}

	void Application::Exit()
	{
		m_Running = false;
	}

	void Application::OnWindowEvent(SDL_Event* event)
	{
		switch(event->window.event)
		{
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			if (event->window.windowID == m_Window->GetWindowID())
			{
				int w, h;
				SDL_GetWindowSize(m_Window->GetWindow(), &w, &h);
				LI_CORE_INFO("Resizing renderer ({0}, {1})", w, h);

				m_Window->OnWindowResize(w, h);
				m_Window->GetContext()->ResizeView(w, h);
				Renderer::Resize(w, h);
#ifndef LI_DIST
				m_ImGuiRenderer->Resize(w, h);
#endif
			}
			break;
		}
	}
}
