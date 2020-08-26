#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/Buffer.h"

namespace li
{
	class ConvertOpenGL
	{
	public:
		static uint32_t BufferUsage(BufferUsage usage);
		static uint32_t BufferTarget(BufferTarget target);
		static uint32_t DrawMode(DrawMode mode);
		static uint32_t WrapType(WrapType type);
		static uint32_t FilterType(FilterType type);
		//static uint32_t FramebufferTarget(FramebufferTarget target);
		//static uint32_t FramebufferAttachment(FramebufferAttachment attachment);
		//static uint32_t ImageFormat(ImageFormat format);
		//static uint32_t InternalImageFormat(InternalImageFormat format);
	};
}