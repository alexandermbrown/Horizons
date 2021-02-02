#pragma once

#include "Lithium/Renderer/Buffer.h"
#include "Lithium/Renderer/RendererEnums.h"

#include "glad/glad.h"

namespace Li
{
	namespace ConvertOpenGL
	{
		uint32_t BufferUsage(Li::BufferUsage usage);
		uint32_t DrawMode(Li::DrawMode mode);
		uint32_t WrapType(Li::WrapType type);
		uint32_t FilterType(Li::FilterType type);
		GLint TextureInternalFormat(int channels);
		GLenum TextureDataFormat(int channels);
	};
}
