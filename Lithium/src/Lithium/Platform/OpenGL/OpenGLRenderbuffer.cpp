#include "lipch.h"
#include "OpenGLRenderbuffer.h"

#include "ConvertOpenGL.h"
#include <glad/glad.h>

namespace li
{
	OpenGLRenderbuffer::OpenGLRenderbuffer()
	{
		glCreateRenderbuffers(1, &m_RendererID);
	}

	OpenGLRenderbuffer::~OpenGLRenderbuffer()
	{
		glDeleteRenderbuffers(1, &m_RendererID);
	}

	void OpenGLRenderbuffer::Bind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
	}

	void OpenGLRenderbuffer::Unbind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void OpenGLRenderbuffer::SetStorage(uint32_t width, uint32_t height, ImageFormat internalFormat)
	{
		m_InternalFormat = ConvertOpenGL::ImageFormat(internalFormat);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
		glRenderbufferStorage(GL_RENDERBUFFER, m_InternalFormat, width, height);
	}

	void OpenGLRenderbuffer::Resize(uint32_t width, uint32_t height)
	{
		glDeleteRenderbuffers(1, &m_RendererID);
		glCreateRenderbuffers(1, &m_RendererID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
		glRenderbufferStorage(GL_RENDERBUFFER, m_InternalFormat, width, height);
	}

	void OpenGLRenderbuffer::AttachToFramebuffer(FramebufferAttachment attachment, FramebufferTarget target) const
	{
		glFramebufferRenderbuffer(ConvertOpenGL::FramebufferTarget(target), ConvertOpenGL::FramebufferAttachment(attachment), GL_RENDERBUFFER, m_RendererID);
	}
}