#include "lipch.h"
#include "OpenGLContext.h"

#include "OpenGLCore.h"

#include <SDL.h>
#include <glad/glad.h>
#include <gl/GL.h>

namespace li
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         LI_CORE_CRITICAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       LI_CORE_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          LI_CORE_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: LI_CORE_TRACE(message); return;
		}

		LI_CORE_ASSERT(false, "Unknown severity level!");
	}

	OpenGLContext::OpenGLContext(SDL_Window* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		m_Context = SDL_GL_CreateContext(windowHandle);

		SDL_GL_MakeCurrent(m_WindowHandle, m_Context);
		LI_CORE_RUN_ASSERT(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress), "Failed to load OpenGL loader!");

		LI_CORE_INFO("OpenGL Info:");
		GLCall(LI_CORE_INFO("    Vendor: {0}", glGetString(GL_VENDOR)));
		GLCall(LI_CORE_INFO("    Renderer: {0}", glGetString(GL_RENDERER)));
		GLCall(LI_CORE_INFO("    Version: OpenGL {0}", glGetString(GL_VERSION)));

#ifdef LI_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		GLCall(glGetIntegerv(GL_MAJOR_VERSION, &versionMajor));
		GLCall(glGetIntegerv(GL_MINOR_VERSION, &versionMinor));

		LI_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Lithium requires at least OpenGL version 4.5!");
#endif

#ifdef LI_DEBUG
		GLCall(glEnable(GL_DEBUG_OUTPUT));
		GLCall(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
		GLCall(glDebugMessageCallback(OpenGLMessageCallback, nullptr));

		GLCall(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE));
#endif

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GLCall(glEnable(GL_DEPTH_TEST));
	}

	OpenGLContext::~OpenGLContext()
	{
		SDL_GL_MakeCurrent(m_WindowHandle, nullptr);
		SDL_GL_DeleteContext(m_Context);
	}

	void OpenGLContext::SwapBuffers()
	{
		SDL_GL_SwapWindow(m_WindowHandle);
	}
}