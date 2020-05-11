#include "lipch.h"
#include "OpenGLWindow.h"

#include "Lithium/Events/WindowEvent.h"
#include "Lithium/Events/MouseEvent.h"
#include "Lithium/Events/KeyEvent.h"

namespace li
{
	OpenGLWindow::OpenGLWindow(const char* title, int width, int height, bool resizable, bool shown)
		: m_Title(title), m_Width(width), m_Height(height)
	{
		int flags = SDL_WINDOW_OPENGL;
		if (resizable) flags |= SDL_WINDOW_RESIZABLE;
		if (shown) flags |= SDL_WINDOW_SHOWN;

		m_Window = SDL_CreateWindow(title, 200, 200, width, height, flags);
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

	void OpenGLWindow::OnWindowResize(const WindowResizeEvent& event)
	{
		m_Width = event.GetWidth();
		m_Height = event.GetHeight();
	}
}