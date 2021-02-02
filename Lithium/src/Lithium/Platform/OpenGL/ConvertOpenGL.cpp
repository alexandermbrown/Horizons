#include "lipch.h"
#include "ConvertOpenGL.h"

namespace Li
{
	uint32_t ConvertOpenGL::BufferUsage(Li::BufferUsage usage)
	{
		switch (usage)
		{
		case BufferUsage::StaticDraw:
			return GL_STATIC_DRAW;
		case BufferUsage::DynamicDraw:
			return GL_DYNAMIC_DRAW;
		default:
			Log::CoreError("Unknown buffer usage {}.", usage);
			return 0;
		}
	}

	uint32_t ConvertOpenGL::DrawMode(Li::DrawMode mode)
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
			Log::CoreError("Unknown draw mode {}.", mode);
			return 0;
		}
	}

	uint32_t ConvertOpenGL::WrapType(Li::WrapType type)
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
			Log::CoreError("Unknown wrap type {}.", type);
			return 0;
		}
	}

	uint32_t ConvertOpenGL::FilterType(Li::FilterType type)
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
			Log::CoreError("Unknown filter type {}.", type);
			return 0;
		}
	}

	GLint ConvertOpenGL::TextureInternalFormat(int channels)
	{
		switch (channels)
		{
		case 1:
			return GL_R8;
		case 2:
			return GL_RG8;
		case 3:
			return GL_RGB8;
		case 4:
			return GL_RGBA8;
		default:
			return 0;
		}
	}

	GLenum ConvertOpenGL::TextureDataFormat(int channels)
	{
		switch (channels)
		{
		case 1:
			return GL_RED;
			break;
		case 2:
			return GL_RG;
		case 3:
			return GL_RGB;
		case 4:
			return GL_RGBA;
		default:
			return 0;
		}
	}

	//uint32_t ConvertOpenGL::FramebufferTarget(Li::FramebufferTarget target)
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
	//		Log::CoreError("Unknown framebuffer target {}.", target);
	//		return 0;
	//	}
	//}

	//uint32_t ConvertOpenGL::FramebufferAttachment(Li::FramebufferAttachment attachment)
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
	//		Log::CoreError("Unknown framebuffer attachment {}.", attachment);
	//		return 0;
	//	}
	//}

	//uint32_t ConvertOpenGL::ImageFormat(Li::ImageFormat format)
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
	//		Log::CoreError("Unknown image format {}.", format);
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
	//		Log::CoreError("Unknown image format {}.", format);
	//		return 0;
	//	}
	//}
}
