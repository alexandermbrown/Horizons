#include "lipch.h"
#include "OpenGLRendererAPI.h"

#include "ConvertOpenGL.h"
#include "glad/glad.h"

#include "OpenGLCore.h"

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

	void OpenGLRendererAPI::InitImpl()
	{
#ifdef LI_DEBUG
		GLCall( glEnable(GL_DEBUG_OUTPUT) );
		GLCall( glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS) );
		GLCall( glDebugMessageCallback(OpenGLMessageCallback, nullptr) );

		GLCall( glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE) );
#endif

		GLCall( glEnable(GL_BLEND) );
		GLCall( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );

		GLCall( glEnable(GL_DEPTH_TEST) );
	}

	void OpenGLRendererAPI::SetViewportImpl(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		GLCall( glViewport(x, y, width, height) );
	}

	void OpenGLRendererAPI::SetClearColorImpl(const glm::vec4& color)
	{
		GLCall( glClearColor(color.r, color.g, color.b, color.a) );
	}

	void OpenGLRendererAPI::ClearImpl()
	{
		GLCall( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
	}

	void OpenGLRendererAPI::SetDepthTestImpl(bool enabled)
	{
		if (enabled) {
			GLCall( glEnable(GL_DEPTH_TEST) );
		}
		else {
			GLCall( glDisable(GL_DEPTH_TEST) );
		}
	}

	void OpenGLRendererAPI::DrawArraysImpl(const Ref<VertexArray>& vertexArray, uint32_t count, DrawMode mode)
	{
		GLCall( glDrawArrays(ConvertOpenGL::DrawMode(mode), 0, (int)count) );
	}

	void OpenGLRendererAPI::DrawIndexedImpl(const Ref<VertexArray>& vertexArray)
	{
		GLCall( glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr) );
	}

	void OpenGLRendererAPI::DrawIndexedImpl(const Ref<VertexArray>& vertexArray, uint32_t count, DrawMode mode)
	{
		GLCall( glDrawElements(ConvertOpenGL::DrawMode(mode), count, GL_UNSIGNED_INT, nullptr) );
	}

	void OpenGLRendererAPI::DrawIndexedInstancedImpl(const Ref<VertexArray>& vertexArray, uint32_t instanceCount, DrawMode mode)
	{
		GLCall( glDrawElementsInstanced(
			ConvertOpenGL::DrawMode(mode),
			vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT,
			nullptr, instanceCount
		) );
	}
}