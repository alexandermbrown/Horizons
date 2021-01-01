#pragma once

#include "Lithium/Renderer/Texture.h"

#include <glad/glad.h>

namespace Li
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(int width, int height, int channels, void* data,
			WrapType wrap_s, WrapType wrap_t, FilterType min_filter, FilterType mag_filter);

		OpenGLTexture2D(const std::string& path, int desired_channels, WrapType wrap_s, WrapType wrap_t,
			FilterType min_filter, FilterType mag_filter);

		OpenGLTexture2D(size_t image_size, const uint8_t* raw_data, int desired_channels,
			WrapType wrap_s, WrapType wrap_t, FilterType min_filter, FilterType mag_filter);

		virtual ~OpenGLTexture2D();

		virtual int GetWidth() const override { return m_Width;  }
		virtual int GetHeight() const override { return m_Height; }
		
		virtual void Bind(uint32_t slot = 0) const override;
		virtual void Resize(int width, int height) override;
		virtual void SetData(const void* data, int width, int height, bool discard) override;
		virtual void AttachToFramebuffer() const override;
		virtual void* GetInternalTexture() const override
		{
			uint64_t id = (uint64_t)m_RendererID;
			return reinterpret_cast<void*>(id);
		}
	private:
		int m_Width;
		int m_Height;
		int m_Channels;
		uint32_t m_RendererID;
		GLint m_InternalFormat;
		GLenum m_DataFormat;
	};
}
