#pragma once

#include "Lithium/Renderer/Framebuffer.h"
#include "Lithium/Platform/OpenGL/OpenGLRenderbuffer.h"

namespace li
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:

		OpenGLFramebuffer(int width, int height);
		virtual ~OpenGLFramebuffer();

		virtual void Bind() const override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() const override;

		virtual void Resize(int width, int height) override;
		inline virtual Ref<Texture2D> GetTexture() const override { return m_Texture; };

	private:

		uint32_t m_RendererID;
		Ref<Texture2D> m_Texture;
		Scope<OpenGLRenderbuffer> m_Renderbuffer;

		int m_Width;
		int m_Height;
	};
}