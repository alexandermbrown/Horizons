#include "lipch.h"
#include "OpenGLContext.h"

#include "OpenGLCore.h"

#include <SDL.h>
#include <glad/glad.h>
#include <gl/GL.h>

namespace li
{
	OpenGLContext::OpenGLContext(SDL_Window* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		m_Context = SDL_GL_CreateContext(windowHandle);
	}

	void OpenGLContext::Init()
	{
		SDL_GL_MakeCurrent(m_WindowHandle, m_Context);
		LI_CORE_ASSERT(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress), "Failed to load OpenGL loader!");

		LI_CORE_INFO("OpenGL Info:");
		GLCall( LI_CORE_INFO("    Vendor: {0}", glGetString(GL_VENDOR)) );
		GLCall( LI_CORE_INFO("    Renderer: {0}", glGetString(GL_RENDERER)) );
		GLCall( LI_CORE_INFO("    Version: OpenGL {0}", glGetString(GL_VERSION)) );

#ifdef LI_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		GLCall( glGetIntegerv(GL_MAJOR_VERSION, &versionMajor) );
		GLCall( glGetIntegerv(GL_MINOR_VERSION, &versionMinor) );

		LI_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Hazel requires at least OpenGL version 4.5!");
#endif
	}

	void OpenGLContext::Shutdown()
	{
		SDL_GL_MakeCurrent(m_WindowHandle, nullptr);
		SDL_GL_DeleteContext(m_Context);
	}

	void OpenGLContext::SwapBuffers()
	{
		SDL_GL_SwapWindow(m_WindowHandle);
	}
}