#include "lipch.h"
#include "OpenGLFramebuffer.h"

#include "ConvertOpenGL.h"
#include "glad/glad.h"

#include "OpenGLCore.h"

namespace li
{
	OpenGLFramebuffer::OpenGLFramebuffer(int width, int height)
		: m_Width(width), m_Height(height)
	{
		GLCall( glGenFramebuffers(1, &m_RendererID) );
		GLCall( glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID) );

		m_Texture = Texture2D::Create(width, height, nullptr,
			WrapType::ClampToEdge, WrapType::ClampToEdge,
			FilterType::Linear, FilterType::Nearest);
		m_Texture->AttachToFramebuffer();

		m_Renderbuffer = CreateScope<OpenGLRenderbuffer>(width, height);
		m_Renderbuffer->AttachToFramebuffer();

		LI_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer not complete!");
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		GLCall( glDeleteFramebuffers(1, &m_RendererID) );
	}

	void OpenGLFramebuffer::Bind() const
	{
		GLCall( glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID) );
		GLCall( glViewport(0, 0, m_Width, m_Height) );
	}

	void OpenGLFramebuffer::SetClearColor(const glm::vec4& color)
	{
		GLCall( glClearColor(color.r, color.g, color.b, color.a) );
	}

	void OpenGLFramebuffer::Clear() const
	{
		GLCall( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
	}

	void OpenGLFramebuffer::Resize(int width, int height)
	{
		GLCall( glDeleteFramebuffers(1, &m_RendererID) );
		GLCall( glGenFramebuffers(1, &m_RendererID) );
		GLCall( glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID) );

		m_Width = width;
		m_Height = height;

		m_Texture->Bind();
		m_Texture->Resize(width, height);
		m_Texture->AttachToFramebuffer();

		m_Renderbuffer->Resize(width, height);
		m_Renderbuffer->AttachToFramebuffer();
	}
}