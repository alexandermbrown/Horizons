#include "lipch.h"
#include "OpenGLWindow.h"

#include "Lithium/Events/WindowEvent.h"
#include "Lithium/Events/MouseEvent.h"
#include "Lithium/Events/KeyEvent.h"
#include "Lithium/Core/Application.h"

namespace li
{
	OpenGLWindow::OpenGLWindow(const char* title, int width, int height, bool resizable, bool shown)
		: m_Title(title), m_Width(width), m_Height(height), m_VSync(false), m_Fullscreen(FullscreenType::Windowed)
	{
		int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;
		if (resizable) flags |= SDL_WINDOW_RESIZABLE;
		if (shown) flags |= SDL_WINDOW_SHOWN;

		m_Window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		LI_CORE_ASSERT(m_Window, "Error creating window.");

		m_Context = CreateRef<OpenGLContext>(m_Window);
		m_Context->Init();
	}

	OpenGLWindow::~OpenGLWindow()
	{
	}

	void OpenGLWindow::Shutdown()
	{
		m_Context->Shutdown();
		SDL_DestroyWindow(m_Window);
	}

	void OpenGLWindow::SwapBuffers()
	{
		m_Context->SwapBuffers();
	}

	void OpenGLWindow::SetVSync(bool enabled)
	{
		if (SDL_GL_SetSwapInterval(enabled ? -1 : 0) < 0)
			LI_CORE_ERROR("Error setting VSync to {0}", enabled);
		m_VSync = enabled;
	}

	void OpenGLWindow::SetFullscreen(FullscreenType type)
	{
		m_Fullscreen = type;
		switch (m_Fullscreen)
		{
		case li::FullscreenType::Windowed:
		{
			LI_CORE_ASSERT(!SDL_SetWindowFullscreen(m_Window, 0), "Failed to turn to windowed.");
			break;
		}
		case li::FullscreenType::Fullscreen:
		{
			LI_CORE_ASSERT(!SDL_SetWindowFullscreen(m_Window, SDL_WINDOW_FULLSCREEN), "Failed to turn to fullscreen.");
			break;
		}
		case li::FullscreenType::FullscreenWindowed:
		{
			LI_CORE_ASSERT(!SDL_SetWindowFullscreen(m_Window, SDL_WINDOW_FULLSCREEN_DESKTOP), "Failed to turn to fullscreen windowed.");
			break;
		}
		default:
			LI_CORE_ERROR("Unknown fullscreen type {}.", type);
			break;
		}
	}

	void OpenGLWindow::OnWindowResize(const WindowResizeEvent& event)
	{
		m_Width = event.GetWidth();
		m_Height = event.GetHeight();
	}
}