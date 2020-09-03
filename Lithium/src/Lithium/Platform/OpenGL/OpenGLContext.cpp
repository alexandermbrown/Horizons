#include "lipch.h"
#include "OpenGLContext.h"

#include "ConvertOpenGL.h"
#include "OpenGLCore.h"

#include "SDL.h"
#include "glad/glad.h"

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

	OpenGLContext::OpenGLContext(SDL_Window* windowHandle, int width, int height)
		: m_WindowHandle(windowHandle), m_Width(width), m_Height(height)
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

		LI_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 2), "Lithium requires at least OpenGL version 4.2!");
#endif
		
#ifdef LI_DEBUG
		GLCall(glEnable(GL_DEBUG_OUTPUT));
		GLCall(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
		GLCall(glDebugMessageCallback(OpenGLMessageCallback, nullptr));

		GLCall(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE));
#endif

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_DepthTest = true;
		GLCall(glEnable(GL_DEPTH_TEST));
	}

	OpenGLContext::~OpenGLContext()
	{
		SDL_GL_MakeCurrent(m_WindowHandle, nullptr);
		SDL_GL_DeleteContext(m_Context);
	}

	void OpenGLContext::BindDefaultRenderTarget()
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		GLCall(glViewport(0, 0, m_Width, m_Height));
	}

	void OpenGLContext::Clear()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void OpenGLContext::DrawArrays(uint32_t vertexCount)
	{
		GLCall(glDrawArrays(ConvertOpenGL::DrawMode(m_DrawMode), 0, (GLsizei)vertexCount));
	}

	void OpenGLContext::DrawIndexed(uint32_t indexCount)
	{
		GLCall(glDrawElements(ConvertOpenGL::DrawMode(m_DrawMode), indexCount, GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLContext::DrawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount)
	{
		GLCall(glDrawElementsInstanced(ConvertOpenGL::DrawMode(m_DrawMode),
			indexCount, GL_UNSIGNED_INT, nullptr, instanceCount));
	}

	void OpenGLContext::ResizeView(int width, int height)
	{
		m_Width = width;
		m_Height = height;
		GLCall(glViewport(0, 0, m_Width, m_Height));
	}

	void OpenGLContext::SetClearColor(const glm::vec4& color)
	{
		GLCall(glClearColor(color.r, color.g, color.b, color.a));
	}

	void OpenGLContext::SetDepthTest(bool enabled)
	{
		if (m_DepthTest == enabled)
			return;

		m_DepthTest = enabled;

		if (enabled) {
			GLCall(glEnable(GL_DEPTH_TEST));
		}
		else {
			GLCall(glDisable(GL_DEPTH_TEST));
		}
	}

	void OpenGLContext::SwapBuffers()
	{
		SDL_GL_SwapWindow(m_WindowHandle);
	}
}