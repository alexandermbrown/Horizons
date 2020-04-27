#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/Renderbuffer.h"

namespace li
{
	class OpenGLRenderbuffer : public Renderbuffer
	{
	public:

		OpenGLRenderbuffer();
		virtual ~OpenGLRenderbuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetStorage(uint32_t width, uint32_t height, ImageFormat internalFormat) override;
		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual void AttachToFramebuffer(
			FramebufferAttachment attachment,
			FramebufferTarget target = FramebufferTarget::Framebuffer
		) const override;

	private:
		uint32_t m_RendererID;
		uint32_t m_InternalFormat;
	};
}