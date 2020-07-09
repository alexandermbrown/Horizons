#include "lipch.h"
#include "OpenGLWindow.h"

#include "Lithium/Core/Application.h"

#include "stb_image.h"

namespace li
{
	OpenGLWindow::OpenGLWindow(const char* title, int width, int height, bool resizable, bool shown, bool borderless)
		: m_Title(title), m_Width(width), m_Height(height), m_VSync(false), m_Fullscreen(FullscreenType::Windowed), m_Icon(nullptr), m_IconData(nullptr)
	{
		int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;
		if (resizable) flags |= SDL_WINDOW_RESIZABLE;
		if (shown)
			flags |= SDL_WINDOW_SHOWN;
		else
			flags |= SDL_WINDOW_HIDDEN;

		if (borderless)
			flags |= SDL_WINDOW_BORDERLESS;

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
		if (m_Icon) 
			SDL_FreeSurface(m_Icon);
		
		if (m_IconData)
			stbi_image_free(m_IconData);

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
			LI_CORE_RUN_ASSERT(!SDL_SetWindowFullscreen(m_Window, 0), "Failed to turn to windowed.");
			break;
		}
		case li::FullscreenType::Fullscreen:
		{
			LI_CORE_RUN_ASSERT(!SDL_SetWindowFullscreen(m_Window, SDL_WINDOW_FULLSCREEN), "Failed to turn to fullscreen.");
			break;
		}
		case li::FullscreenType::FullscreenWindowed:
		{
			LI_CORE_RUN_ASSERT(!SDL_SetWindowFullscreen(m_Window, SDL_WINDOW_FULLSCREEN_DESKTOP), "Failed to turn to fullscreen windowed.");
			break;
		}
		default:
			LI_CORE_ERROR("Unknown fullscreen type {}.", type);
			break;
		}
	}

	void OpenGLWindow::SetBordered(bool bordered)
	{
		SDL_SetWindowBordered(m_Window, bordered ? SDL_TRUE : SDL_FALSE);
	}

	void OpenGLWindow::SetResizable(bool resizable)
	{
		SDL_SetWindowResizable(m_Window, resizable ? SDL_TRUE : SDL_FALSE);
	}

	void OpenGLWindow::Show()
	{
		SDL_ShowWindow(m_Window);
	}

	void OpenGLWindow::Hide()
	{
		SDL_HideWindow(m_Window);
	}

	void OpenGLWindow::SetSize(int width, int height)
	{
		SDL_SetWindowSize(m_Window, width, height);
		m_Width = width;
		m_Height = height;
	}

	void OpenGLWindow::SetPosition(int x, int y)
	{
		SDL_SetWindowPosition(m_Window, x, y);
	}

	void OpenGLWindow::SetIcon(const std::string& path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(0);

		m_IconData = stbi_load(path.c_str(), &width, &height, &channels, 0);

		LI_CORE_ASSERT(m_IconData, "Failed to load image!");

		int pitch = 0;
		uint32_t pixel_format = 0;
		if (channels == 4)
		{
			pitch = 4 * width;
			pixel_format = SDL_PIXELFORMAT_RGBA32;
		}
		else if (channels == 3)
		{
			pitch = 3 * width;
			pixel_format = SDL_PIXELFORMAT_RGB24;
		}
		else
		{
			LI_CORE_ERROR("Image must have 3 or 4 chanels.");
			return;
		}

		m_Icon = SDL_CreateRGBSurfaceWithFormatFrom(m_IconData, width, height, channels * 8, pitch, pixel_format);

		if (!m_Icon)
		{
			LI_CORE_ERROR("Failed to create SDL_Surface.");
			return;
		}

		SDL_SetWindowIcon(m_Window, m_Icon);

		LI_CORE_INFO("Set icon to {}", path);
	}

	void OpenGLWindow::OnWindowEvent(SDL_Event* event)
	{
		if (event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
		{
			m_Width = event->window.data1;
			m_Height = event->window.data2;
		}
	}
}