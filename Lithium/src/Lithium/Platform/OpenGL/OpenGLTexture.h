#pragma once

#include "Lithium/Renderer/Texture.h"

#include <glad/glad.h>

namespace li
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(int width, int height, void* data,
			WrapType wrapS, WrapType wrapT,
			FilterType minFilter, FilterType magFilter);
		OpenGLTexture2D(const std::string& path,
			WrapType wrapS, WrapType wrapT,
			FilterType minFilter, FilterType magFilter);
		OpenGLTexture2D(size_t imageSize, uint8_t* rawData,
			WrapType wrapS = WrapType::ClampToEdge, WrapType wrapT = WrapType::ClampToEdge,
			FilterType minFilter = FilterType::Linear, FilterType magFilter = FilterType::Linear);

		virtual ~OpenGLTexture2D();

		virtual int GetWidth() const override { return m_Width;  }
		virtual int GetHeight() const override { return m_Height; }
		
		virtual void Resize(int width, int height) override;
		virtual void AttachToFramebuffer(
			FramebufferAttachment attachment,
			FramebufferTarget target = FramebufferTarget::Framebuffer
		) const override;

		virtual void Bind(uint32_t slot = 0) const override;
	private:
		int m_Width;
		int m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};
}
