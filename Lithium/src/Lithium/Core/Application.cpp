#include "lipch.h"
#include "Application.h"

#include "Lithium/Core/Core.h"
#include "Lithium/Core/Log.h"
#include "Lithium/Events/EventDispatcher.h"
#include "Lithium/Resources/ResourceManager.h"
#include "Lithium/Audio/AudioManager.h"
#include "Lithium/Localization/Localization.h"
#include <SDL.h>
#include <chrono>

#include "imgui.h"

#define LI_MIN_MS_PER_FRAME 2

namespace li 
{

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Running(false), m_LayerStack(), m_LastTicks(0), m_EventHandled(false), m_Input(), m_FocusedLayer(nullptr)
	{
		LI_CORE_ASSERT(!s_Instance, "Instance of Application already exists!");
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

		m_EventCallbackFn = LI_BIND_EVENT_FN(Application::OnEvent);

		m_Window = Window::Create("Lithium Engine", 1280, 720, true, true, Renderer::GetAPI());

		m_ImGuiRenderer = CreateScope<ImGuiRenderer>();

		Localization::Init();
		Renderer::Init();
		AudioManager::Init();
	}

	Application::~Application()
	{
		m_LayerStack.Clear();
		ResourceManager::Shutdown();
		AudioManager::Shutdown();
		Renderer::Shutdown();
		m_Window->Shutdown();
		SDL_Quit();
	}

	void Application::Run()
	{
		m_Running = true;

		while (m_Running)
		{
			SDL_Event sdlEvent;
			while (SDL_PollEvent(&sdlEvent))
			{
				OnEvent(&sdlEvent);
			}
			
			unsigned int ticks = SDL_GetTicks();
			unsigned int deltaTicks = ticks - m_LastTicks;
			if (deltaTicks < LI_MIN_MS_PER_FRAME)
				continue;

			m_LastTicks = ticks;

			bool reachedFocused = false;
			if (m_FocusedLayer)
				m_Input.Disable();

			for (Layer* layer : m_LayerStack)
			{
				if (m_FocusedLayer == layer && !reachedFocused)
				{
					m_Input.Enable();
					reachedFocused = true;
				}

				layer->OnUpdate((float)deltaTicks * 0.001f);
			}

			reachedFocused = false;
			if (m_FocusedLayer)
				m_Input.Disable();

			m_ImGuiRenderer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				if (m_FocusedLayer == layer && !reachedFocused)
				{
					m_Input.Enable();
					reachedFocused = true;
				}
				layer->OnImGuiRender();
			}
			m_ImGuiRenderer->End();

			m_Window->SwapBuffers();
		}
	}

	void Application::OnEvent(SDL_Event* event)
	{
		m_Input.OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch(SDL_WINDOWEVENT, LI_BIND_EVENT_FN(Application::OnWindowEvent));

		m_ImGuiRenderer->OnEvent(event);

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (m_EventHandled) {
				m_EventHandled = false;
				break;
			}
			(*it)->OnEvent(event);
		}
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

	void Application::OnWindowEvent(SDL_Event* event)
	{
		m_Window->OnWindowEvent(event);
		switch(event->window.event)
		{
		case SDL_WINDOWEVENT_CLOSE:
			m_Running = false;
			break;
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			int w, h;
			SDL_GL_GetDrawableSize(m_Window->GetWindow(), &w, &h);
			LI_CORE_TRACE("Resizing renderer: {0}, {1}", w, h);

			RendererAPI::SetViewport(0, 0, w, h);
			Renderer::Resize(w, h);
			m_ImGuiRenderer->Resize(w, h);

			break;
		}
	}
}