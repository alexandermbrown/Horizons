#include "lipch.h"
#include "OpenGLRendererAPI.h"

#include "ConvertOpenGL.h"
#include "glad/glad.h"

#include "OpenGLCore.h"

namespace li
{
	void OpenGLRendererAPI::ResizeViewImpl(int width, int height)
	{
		m_Width = width;
		m_Height = height;
		GLCall( glViewport(0, 0, m_Width, m_Height) );
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
		if (m_DepthTest == enabled)
			return;

		m_DepthTest = enabled;

		if (enabled) {
			GLCall( glEnable(GL_DEPTH_TEST) );
		}
		else {
			GLCall( glDisable(GL_DEPTH_TEST) );
		}
	}

	void OpenGLRendererAPI::DrawArraysImpl(uint32_t vertexCount)
	{
		GLCall( glDrawArrays(ConvertOpenGL::DrawMode(m_DrawMode), 0, (GLsizei)vertexCount) );
	}

	void OpenGLRendererAPI::DrawIndexedImpl(uint32_t indexCount)
	{
		GLCall( glDrawElements(ConvertOpenGL::DrawMode(m_DrawMode), indexCount, GL_UNSIGNED_INT, nullptr) );
	}

	void OpenGLRendererAPI::DrawIndexedInstancedImpl(uint32_t indexCount, uint32_t instanceCount)
	{
		GLCall( glDrawElementsInstanced(ConvertOpenGL::DrawMode(m_DrawMode), 
			indexCount, GL_UNSIGNED_INT, nullptr, instanceCount) );
	}
	
	void OpenGLRendererAPI::BindDefaultRenderTargetImpl()
	{
		GLCall( glBindFramebuffer(GL_FRAMEBUFFER, 0) );
		GLCall( glViewport(0, 0, m_Width, m_Height) );
	}
}