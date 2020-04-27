#pragma once

#include "Lithium/Renderer/Framebuffer.h"

namespace li
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:

		OpenGLFramebuffer(FramebufferTarget target = FramebufferTarget::Framebuffer);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AttachTexture(
			Ref<Texture2D> texture, 
			FramebufferAttachment attachment
		) override;

		virtual void AttachRenderbuffer(
			Ref<Renderbuffer> renderBuffer,
			FramebufferAttachment attachmen
		) override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		inline virtual Ref<Texture2D> GetTexture() const override { return m_Texture; };
		virtual bool IsComplete() const override;

	private:

		uint32_t m_RendererID;
		Ref<Texture2D> m_Texture;
		Ref<Renderbuffer> m_Renderbuffer;

		FramebufferTarget m_Target;
		uint32_t m_GLTarget;

		FramebufferAttachment m_TextureAttachment;
		FramebufferAttachment m_RenderbufferAttachment;
	};
}