#include "lipch.h"
#include "OpenGLRenderbuffer.h"

#include "ConvertOpenGL.h"
#include "glad/glad.h"

#include "OpenGLCore.h"

namespace li
{
	OpenGLRenderbuffer::OpenGLRenderbuffer()
	{
		GLCall( glCreateRenderbuffers(1, &m_RendererID) );
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

	void OpenGLRenderbuffer::SetStorage(uint32_t width, uint32_t height, ImageFormat internalFormat)
	{
		m_InternalFormat = ConvertOpenGL::ImageFormat(internalFormat);
		GLCall( glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID) );
		GLCall( glRenderbufferStorage(GL_RENDERBUFFER, m_InternalFormat, width, height) );
	}

	void OpenGLRenderbuffer::Resize(uint32_t width, uint32_t height)
	{
		GLCall( glDeleteRenderbuffers(1, &m_RendererID) );
		GLCall( glCreateRenderbuffers(1, &m_RendererID) );
		GLCall( glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID) );
		GLCall( glRenderbufferStorage(GL_RENDERBUFFER, m_InternalFormat, width, height) );
	}

	void OpenGLRenderbuffer::AttachToFramebuffer(FramebufferAttachment attachment, FramebufferTarget target) const
	{
		GLCall( glFramebufferRenderbuffer(
			ConvertOpenGL::FramebufferTarget(target),
			ConvertOpenGL::FramebufferAttachment(attachment),
			GL_RENDERBUFFER, m_RendererID
		) );
	}
}