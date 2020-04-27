#include "lipch.h"
#include "OpenGLRendererAPI.h"

#include "ConvertOpenGL.h"
#include <glad/glad.h>

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
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewportImpl(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColorImpl(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::ClearImpl()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetDepthTestImpl(bool enabled)
	{
		if (enabled)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::DrawIndexedImpl(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawIndexedInstancedImpl(const Ref<VertexArray>& vertexArray, uint32_t instanceCount, DrawMode mode)
	{
		glDrawElementsInstanced(ConvertOpenGL::DrawMode(mode),
			vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT,
			nullptr, instanceCount);
	}
}