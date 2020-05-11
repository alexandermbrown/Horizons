#include "lipch.h"
#include "OpenGLFramebuffer.h"

#include "ConvertOpenGL.h"
#include "glad/glad.h"

#include "OpenGLCore.h"

namespace li
{
	OpenGLFramebuffer::OpenGLFramebuffer(FramebufferTarget target)
		: m_Target(target), m_GLTarget(ConvertOpenGL::FramebufferTarget(m_Target))
	{
		GLCall( glCreateFramebuffers(1, &m_RendererID) );
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		GLCall( glDeleteFramebuffers(1, &m_RendererID) );
	}

	void OpenGLFramebuffer::Bind() const
	{
		GLCall( glBindFramebuffer(m_GLTarget, m_RendererID) );
	}

	void OpenGLFramebuffer::Unbind() const
	{
		GLCall( glBindFramebuffer(m_GLTarget, 0) );
	}

	void OpenGLFramebuffer::AttachTexture(Ref<Texture2D> texture, FramebufferAttachment attachment)
	{
		m_Texture = texture;
		m_TextureAttachment = attachment;

		m_Texture->AttachToFramebuffer(attachment, m_Target);
	}

	void OpenGLFramebuffer::AttachRenderbuffer(Ref<Renderbuffer> renderBuffer, FramebufferAttachment attachment)
	{
		m_Renderbuffer = renderBuffer;
		m_RenderbufferAttachment = attachment;

		m_Renderbuffer->AttachToFramebuffer(attachment, m_Target);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		GLCall( glDeleteFramebuffers(1, &m_RendererID) );
		GLCall( glCreateFramebuffers(1, &m_RendererID) );
		GLCall( glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID) );

		if (m_Texture) 
		{
			m_Texture->Bind();
			m_Texture->Resize(width, height);
			m_Texture->AttachToFramebuffer(m_TextureAttachment, m_Target);
		}

		if (m_Renderbuffer) 
		{
			m_Renderbuffer->Resize(width, height);
			m_Renderbuffer->AttachToFramebuffer(m_RenderbufferAttachment, m_Target);
		}
	}

	bool OpenGLFramebuffer::IsComplete() const
	{
		return glCheckFramebufferStatus(m_GLTarget);
	}
}