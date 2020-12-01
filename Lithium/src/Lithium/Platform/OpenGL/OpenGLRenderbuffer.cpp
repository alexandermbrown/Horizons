#include "lipch.h"
#include "OpenGLRenderbuffer.h"

#include "ConvertOpenGL.h"
#include "glad/glad.h"

#include "OpenGLCore.h"

namespace li
{
	OpenGLRenderbuffer::OpenGLRenderbuffer(uint32_t width, uint32_t height)
	{
		glGenRenderbuffers(1, &m_RendererID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	}

	OpenGLRenderbuffer::~OpenGLRenderbuffer()
	{
		GLCall( glDeleteRenderbuffers(1, &m_RendererID) );
	}

	void OpenGLRenderbuffer::Bind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
	}

	void OpenGLRenderbuffer::Unbind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void OpenGLRenderbuffer::Resize(uint32_t width, uint32_t height)
	{
		glDeleteRenderbuffers(1, &m_RendererID);
		glGenRenderbuffers(1, &m_RendererID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	}

	void OpenGLRenderbuffer::AttachToFramebuffer() const
	{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RendererID);
	}
}