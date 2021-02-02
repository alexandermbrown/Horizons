#include "lipch.h"
#include "OpenGLContext.h"

#include "Lithium/Core/Assert.h"
#include "ConvertOpenGL.h"
#include "OpenGLCore.h"

#include "SDL.h"
#include "glad/glad.h"

#include <stdexcept>
#include <sstream>

namespace Li
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* user_param)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         Log::CoreFatal(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       Log::CoreError(message); return;
		case GL_DEBUG_SEVERITY_LOW:          Log::CoreWarn(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: Log::CoreTrace(message); return;
		}

		LI_CORE_ASSERT(false, "Unknown severity level!");
	}

	OpenGLContext::OpenGLContext(SDL_Window* window_handle, int width, int height)
		: m_WindowHandle(window_handle), m_Width(width), m_Height(height)
	{
		m_Context = SDL_GL_CreateContext(window_handle);

		SDL_GL_MakeCurrent(m_WindowHandle, m_Context);
		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
			throw std::runtime_error("Failed to load OpenGL functions.");

		Log::CoreInfo("OpenGL Info:");
		GLCall(Log::CoreInfo("    Vendor: {0}", glGetString(GL_VENDOR)));
		GLCall(Log::CoreInfo("    Renderer: {0}", glGetString(GL_RENDERER)));
		GLCall(Log::CoreInfo("    Version: OpenGL {0}", glGetString(GL_VERSION)));
		
		// Verify OpenGL version.
		int version_major;
		int version_minor;
		GLCall(glGetIntegerv(GL_MAJOR_VERSION, &version_major));
		GLCall(glGetIntegerv(GL_MINOR_VERSION, &version_minor));

#ifdef LI_DEBUG
		constexpr int RequiredMajor = 4;
		constexpr int RequiredMinor = 3;
#else
		constexpr int RequiredMajor = 4;
		constexpr int RequiredMinor = 2;
#endif
		bool good_version = version_major > RequiredMajor || (version_major == RequiredMajor && version_minor >= RequiredMinor);
		if (!good_version)
		{
			std::stringstream stream;
			stream << "OpenGL version " << RequiredMajor << "." << RequiredMinor << " is required.";
			throw std::runtime_error(stream.str());
		}
		
#ifdef LI_DEBUG
		GLCall(glEnable(GL_DEBUG_OUTPUT));
		GLCall(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
		GLCall(glDebugMessageCallback(OpenGLMessageCallback, nullptr));

		GLCall(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE));
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_DepthTest = true;
		glEnable(GL_DEPTH_TEST);
	}

	OpenGLContext::~OpenGLContext()
	{
		SDL_GL_MakeCurrent(m_WindowHandle, nullptr);
		SDL_GL_DeleteContext(m_Context);
	}

	void OpenGLContext::BindDefaultRenderTarget()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, m_Width, m_Height);
	}

	void OpenGLContext::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLContext::DrawArrays(uint32_t vertexCount)
	{
		glDrawArrays(ConvertOpenGL::DrawMode(m_DrawMode), 0, (GLsizei)vertexCount);
	}

	void OpenGLContext::DrawIndexed(uint32_t indexCount)
	{
		glDrawElements(ConvertOpenGL::DrawMode(m_DrawMode), indexCount, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLContext::DrawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount)
	{
		glDrawElementsInstanced(ConvertOpenGL::DrawMode(m_DrawMode),
			indexCount, GL_UNSIGNED_INT, nullptr, instanceCount);
	}

	void OpenGLContext::ResizeView(int width, int height)
	{
		m_Width = width;
		m_Height = height;
		glViewport(0, 0, m_Width, m_Height);
	}

	void OpenGLContext::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLContext::SetDepthTest(bool enabled)
	{
		if (m_DepthTest == enabled)
			return;

		m_DepthTest = enabled;

		if (enabled) {
			glEnable(GL_DEPTH_TEST);
		}
		else {
			glDisable(GL_DEPTH_TEST);
		}
	}

	void OpenGLContext::SwapBuffers()
	{
		SDL_GL_SwapWindow(m_WindowHandle);
	}
}
