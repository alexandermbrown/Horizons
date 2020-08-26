#include "lipch.h"
#include "ConvertOpenGL.h"

#include <glad/glad.h>

namespace li
{
	uint32_t ConvertOpenGL::BufferUsage(li::BufferUsage usage)
	{
		switch (usage)
		{
		case BufferUsage::StaticDraw:
			return GL_STATIC_DRAW;
		case BufferUsage::DynamicDraw:
			return GL_DYNAMIC_DRAW;
		default:
			LI_CORE_ERROR("Unknown buffer usage {}.", usage);
			return 0;
		}
	}

	uint32_t ConvertOpenGL::BufferTarget(li::BufferTarget target)
	{
		switch (target)
		{
		case BufferTarget::ArrayBuffer:
			return GL_ARRAY_BUFFER;
		case BufferTarget::ElementArrayBuffer:
			return GL_ELEMENT_ARRAY_BUFFER;
		default:
			LI_CORE_ERROR("Unknown buffer target {}.", target);
			return 0;
		}
	}

	uint32_t ConvertOpenGL::DrawMode(li::DrawMode mode)
	{
		switch (mode)
		{
		case DrawMode::Points:
			return GL_POINTS;
		case DrawMode::Lines:
			return GL_LINES;
		case DrawMode::LineStrip:
			return GL_LINE_STRIP;
		case DrawMode::Triangles:
			return GL_TRIANGLES;
		case DrawMode::TriangleStrip:
			return GL_TRIANGLE_STRIP;
		default:
			LI_CORE_ERROR("Unknown draw mode {}.", mode);
			return 0;
		}
	}

	uint32_t ConvertOpenGL::WrapType(li::WrapType type)
	{
		switch (type)
		{
		case WrapType::Repeat:
			return GL_REPEAT;
		case WrapType::MirroredRepeat:
			return GL_MIRRORED_REPEAT;
		case WrapType::ClampToEdge:
			return GL_CLAMP_TO_EDGE;
		case WrapType::ClampToBorder:
			return GL_CLAMP_TO_BORDER;
		default:
			LI_CORE_ERROR("Unknown wrap type {}.", type);
			return 0;
		}
	}

	uint32_t ConvertOpenGL::FilterType(li::FilterType type)
	{
		switch (type)
		{
		case FilterType::Nearest:
			return GL_NEAREST;
		case FilterType::Linear:
			return GL_LINEAR;
		case FilterType::NearestMipmapNearest:
			return GL_NEAREST_MIPMAP_NEAREST;
		case FilterType::LinearMipmapNearest:
			return GL_LINEAR_MIPMAP_NEAREST;
		case FilterType::NearestMipmapLinear:
			return GL_NEAREST_MIPMAP_LINEAR;
		default:
			LI_CORE_ERROR("Unknown filter type {}.", type);
			return 0;
		}
	}

	//uint32_t ConvertOpenGL::FramebufferTarget(li::FramebufferTarget target)
	//{
	//	switch (target)
	//	{
	//	case FramebufferTarget::Framebuffer:
	//		return GL_FRAMEBUFFER;
	//	case FramebufferTarget::ReadFramebuffer:
	//		return GL_READ_FRAMEBUFFER;
	//	case FramebufferTarget::DrawFramebuffer:
	//		return GL_DRAW_FRAMEBUFFER;
	//	default:
	//		LI_CORE_ERROR("Unknown framebuffer target {}.", target);
	//		return 0;
	//	}
	//}

	//uint32_t ConvertOpenGL::FramebufferAttachment(li::FramebufferAttachment attachment)
	//{
	//	switch (attachment)
	//	{
	//	case FramebufferAttachment::ColorAttachment0:
	//		return GL_COLOR_ATTACHMENT0;
	//	case FramebufferAttachment::ColorAttachment1:
	//		return GL_COLOR_ATTACHMENT1;
	//	case FramebufferAttachment::ColorAttachment2:
	//		return GL_COLOR_ATTACHMENT2;
	//	case FramebufferAttachment::ColorAttachment3:
	//		return GL_COLOR_ATTACHMENT3;
	//	case FramebufferAttachment::ColorAttachment4:
	//		return GL_COLOR_ATTACHMENT4;
	//	case FramebufferAttachment::ColorAttachment5:
	//		return GL_COLOR_ATTACHMENT5;
	//	case FramebufferAttachment::ColorAttachment6:
	//		return GL_COLOR_ATTACHMENT6;
	//	case FramebufferAttachment::ColorAttachment7:
	//		return GL_COLOR_ATTACHMENT7;
	//	case FramebufferAttachment::DepthAttachment:
	//		return GL_DEPTH_ATTACHMENT;
	//	case FramebufferAttachment::StencilAttachment:
	//		return GL_STENCIL_ATTACHMENT;
	//	case FramebufferAttachment::DepthStencilAttachment:
	//		return GL_DEPTH_STENCIL_ATTACHMENT;
	//	default:
	//		LI_CORE_ERROR("Unknown framebuffer attachment {}.", attachment);
	//		return 0;
	//	}
	//}

	//uint32_t ConvertOpenGL::ImageFormat(li::ImageFormat format)
	//{
	//	switch (format)
	//	{
	//	case ImageFormat::RGB:
	//		return GL_RGB;
	//	case ImageFormat::RGBA:
	//		return GL_RGBA;
	//	case ImageFormat::DepthStencil:
	//		return GL_DEPTH_STENCIL;
	//	default:
	//		LI_CORE_ERROR("Unknown image format {}.", format);
	//		return 0;
	//	}
	//}
	//uint32_t ConvertOpenGL::InternalImageFormat(InternalImageFormat format)
	//{
	//	switch (format)
	//	{
	//	case InternalImageFormat::RGB8:
	//		return GL_RGB8;
	//	case InternalImageFormat::RGBA8:
	//		return GL_RGBA8;
	//	case InternalImageFormat::Depth24Stencil8:
	//		return GL_DEPTH24_STENCIL8;
	//	default:
	//		LI_CORE_ERROR("Unknown image format {}.", format);
	//		return 0;
	//	}
	//}
}