#include "lipch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include "ConvertOpenGL.h"
#include "OpenGLCore.h"

namespace li
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height,
		WrapType wrapS, WrapType wrapT,
		FilterType minFilter, FilterType magFilter)
		: m_Width(width), m_Height(height)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		GLCall( glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID) );
		GLCall( glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height) );

		GLCall( glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, ConvertOpenGL::FilterType(minFilter)) );
		GLCall( glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, ConvertOpenGL::FilterType(magFilter)) );

		GLCall( glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, ConvertOpenGL::WrapType(wrapS)) );
		GLCall( glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, ConvertOpenGL::WrapType(wrapT)) );
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path,
		WrapType wrapS, WrapType wrapT,
		FilterType minFilter, FilterType magFilter)
		: m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = nullptr;
		data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		
		LI_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		LI_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		GLCall( glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID) );
		GLCall( glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height) );

		GLCall( glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, ConvertOpenGL::FilterType(minFilter)) );
		GLCall( glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, ConvertOpenGL::FilterType(magFilter)) );

		GLCall( glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, ConvertOpenGL::WrapType(wrapS)) );
		GLCall( glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, ConvertOpenGL::WrapType(wrapT)) );

		GLCall( glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data) );

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(size_t imageSize, uint8_t* rawData, WrapType wrapS, WrapType wrapT, FilterType minFilter, FilterType magFilter)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = nullptr;
		data = stbi_load_from_memory(rawData, (int)imageSize, &width, &height, &channels, 0);

		LI_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		LI_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		GLCall( glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID) );
		GLCall( glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height) );

		GLCall( glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, ConvertOpenGL::FilterType(minFilter)) );
		GLCall( glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, ConvertOpenGL::FilterType(magFilter)) );

		GLCall( glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, ConvertOpenGL::WrapType(wrapS)) );
		GLCall( glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, ConvertOpenGL::WrapType(wrapT)) );

		GLCall( glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data) );

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		GLCall( glDeleteTextures(1, &m_RendererID) );
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		LI_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		GLCall( glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data) );
	}

	void OpenGLTexture2D::Resize(uint32_t width, uint32_t height)
	{
		GLCall( glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_DataFormat, GL_UNSIGNED_BYTE, nullptr) );
		m_Width = width;
		m_Height = height;
	}

	void OpenGLTexture2D::AttachToFramebuffer(FramebufferAttachment attachment, FramebufferTarget target) const
	{
		GLCall( glFramebufferTexture2D(ConvertOpenGL::FramebufferTarget(target), ConvertOpenGL::FramebufferAttachment(attachment), GL_TEXTURE_2D, m_RendererID, 0) );
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		GLCall( glBindTextureUnit(slot, m_RendererID) );
	}
}