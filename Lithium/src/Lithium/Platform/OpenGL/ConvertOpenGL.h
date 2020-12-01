#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/Buffer.h"

#include <glad/glad.h>

namespace li
{
	namespace ConvertOpenGL
	{
		uint32_t BufferUsage(li::BufferUsage usage);
		uint32_t DrawMode(li::DrawMode mode);
		uint32_t WrapType(li::WrapType type);
		uint32_t FilterType(li::FilterType type);
		GLint TextureInternalFormat(int channels);
		GLenum TextureDataFormat(int channels);
	};
}