#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/GraphicsContext.h"
#include <SDL.h>

namespace li
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(SDL_Window* windowHandle);
		virtual ~OpenGLContext();

		void SwapBuffers();

		virtual inline SDL_GLContext* GetGLContext() { return &m_Context; }

	private:
		SDL_Window* m_WindowHandle;
		SDL_GLContext m_Context;
	};
}