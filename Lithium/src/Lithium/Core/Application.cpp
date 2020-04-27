#include "lipch.h"
#include "Application.h"

#include "Lithium/Core/Core.h"
#include "Lithium/Core/Log.h"
#include "Lithium/Core/SDLEvent.h"
#include "Lithium/Resources/ResourceManager.h"
#include <SDL.h>
#include <chrono>

#include <imgui.h>

#define LI_MIN_TICKS 2

namespace li 
{

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Running(false), m_LayerStack(), m_LastTicks(0)
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

		m_Window = Window::Create("Lithium Engine", 1280, 720, true, true, Renderer::GetAPI());

		m_ImGuiRenderer = CreateScope<ImGuiRenderer>();

		Renderer::Init();

		ResourceManager::Init("resources.lab");
	}

	Application::~Application()
	{
		
	}

	void Application::Run()
	{
		m_Running = true;

		while (m_Running)
		{
			SDL_Event sdlEvent;
			while (SDL_PollEvent(&sdlEvent))
			{
				m_ImGuiRenderer->OnSDLEvent(&sdlEvent);
				// Capture ImGui keyboard and mouse input.
				if (m_ImGuiRenderer->WantCapture(sdlEvent))
					continue;
				
				SDLEvent::Broadcast(sdlEvent, LI_BIND_EVENT_FN(Application::OnEvent));
			}
			
			unsigned int ticks = SDL_GetTicks();
			unsigned int deltaTicks = ticks - m_LastTicks;
			if (deltaTicks < LI_MIN_TICKS)
				continue;

			m_LastTicks = ticks;

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate((float)deltaTicks * 0.001f);
			}

			m_ImGuiRenderer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiRenderer->End();

			m_Window->SwapBuffers();
		}
	}

	void Application::OnEvent(Event& e)
	{
		//LI_CORE_TRACE(e.ToString());

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(LI_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(LI_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvent(e);
			if (e.Handled)
				break;
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

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		LI_CORE_TRACE("Resizing window to: {0}, {1}", e.GetWidth(), e.GetHeight());
		RendererAPI::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
		Renderer::Resize(e.GetWidth(), e.GetHeight());

		return false;
	}
}