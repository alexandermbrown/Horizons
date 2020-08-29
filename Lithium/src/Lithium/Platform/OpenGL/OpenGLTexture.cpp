#include "lipch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include "ConvertOpenGL.h"
#include "OpenGLCore.h"

namespace li
{
	OpenGLTexture2D::OpenGLTexture2D(int width, int height, void* data,
		WrapType wrapS, WrapType wrapT,
		FilterType minFilter, FilterType magFilter, int channels)
		: m_Width(width), m_Height(height)
	{
		switch (channels)
		{
		case 1:
			m_InternalFormat = GL_R8;
			m_DataFormat = GL_RED;
			break;
		case 2:
			m_InternalFormat = GL_RG8;
			m_DataFormat = GL_RG;
			break;
		case 3:
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
			break;
		case 4:
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
			break;
		}
		

		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ConvertOpenGL::FilterType(minFilter)));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ConvertOpenGL::FilterType(magFilter)));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ConvertOpenGL::WrapType(wrapS)));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ConvertOpenGL::WrapType(wrapT)));
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path,
		WrapType wrapS, WrapType wrapT,
		FilterType minFilter, FilterType magFilter)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = nullptr;
		data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		
		LI_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		if (channels == 4)
		{
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
		}
		else LI_CORE_ASSERT(false, "Format not supported!");

		GLCall( glGenTextures(1, &m_RendererID) );
		GLCall( glBindTexture(GL_TEXTURE_2D, m_RendererID) );
		GLCall( glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data) );

		GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ConvertOpenGL::FilterType(minFilter)) );
		GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ConvertOpenGL::FilterType(magFilter)) );

		GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ConvertOpenGL::WrapType(wrapS)) );
		GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ConvertOpenGL::WrapType(wrapT)) );

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(size_t imageSize, const uint8_t* rawData, WrapType wrapS, WrapType wrapT, FilterType minFilter, FilterType magFilter)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = nullptr;
		data = stbi_load_from_memory(rawData, (int)imageSize, &width, &height, &channels, 0);

		LI_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;
		
		if (channels == 4)
		{
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
		}
		else LI_CORE_ASSERT(false, "Format not supported!");

		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ConvertOpenGL::FilterType(minFilter)));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ConvertOpenGL::FilterType(magFilter)));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ConvertOpenGL::WrapType(wrapS)));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ConvertOpenGL::WrapType(wrapT)));

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		GLCall( glDeleteTextures(1, &m_RendererID) );
	}

	void OpenGLTexture2D::Resize(int width, int height)
	{
		GLCall( glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_DataFormat, GL_UNSIGNED_BYTE, nullptr) );
		m_Width = width;
		m_Height = height;
	}

	void OpenGLTexture2D::AttachToFramebuffer() const
	{
		GLCall( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RendererID, 0) );
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		GLCall( glActiveTexture(GL_TEXTURE0 + slot) );
		GLCall( glBindTexture(GL_TEXTURE_2D, m_RendererID) );
	}
}