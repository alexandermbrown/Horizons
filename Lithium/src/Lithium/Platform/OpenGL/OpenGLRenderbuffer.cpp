#include "lipch.h"
#include "OpenGLRenderbuffer.h"

#include "ConvertOpenGL.h"
#include "glad/glad.h"

#include "OpenGLCore.h"

namespace li
{
	OpenGLRenderbuffer::OpenGLRenderbuffer(uint32_t width, uint32_t height)
	{
		GLCall( glGenRenderbuffers(1, &m_RendererID) );
		GLCall( glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID) );
		GLCall( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height) );
	}

	OpenGLRenderbuffer::~OpenGLRenderbuffer()
	{
		GLCall( glDeleteRenderbuffers(1, &m_RendererID) );
	}

	void OpenGLRenderbuffer::Bind() const
	{
		GLCall( glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID) );
	}

	void OpenGLRenderbuffer::Unbind() const
	{
		GLCall( glBindRenderbuffer(GL_RENDERBUFFER, 0) );
	}

	void OpenGLRenderbuffer::Resize(uint32_t width, uint32_t height)
	{
		GLCall( glDeleteRenderbuffers(1, &m_RendererID) );
		GLCall( glGenRenderbuffers(1, &m_RendererID) );
		GLCall( glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID) );
		GLCall( glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height) );
	}

	void OpenGLRenderbuffer::AttachToFramebuffer() const
	{
		GLCall( glFramebufferRenderbuffer(GL_FRAMEBUFFER,
			GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RendererID) );
	}
}