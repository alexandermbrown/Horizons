#pragma once

namespace li
{
	enum class BufferUsage
	{
		Error,
		StaticDraw,
		DynamicDraw
	};

	enum class BufferTarget
	{
		Error,
		ArrayBuffer,
		ElementArrayBuffer
	};

	enum class DrawMode
	{
		Error,
		Points,
		Lines,
		LineStrip,
		Triangles,
		TriangleStrip
	};

	enum class WrapType
	{
		Error,
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder
	};

	enum class FilterType
	{
		Error,
		Nearest,
		Linear,
		NearestMipmapNearest,
		LinearMipmapNearest,
		NearestMipmapLinear
	};

	enum class FramebufferTarget
	{
		Framebuffer,
		ReadFramebuffer,
		DrawFramebuffer
	};

	enum class FramebufferAttachment
	{
		ColorAttachment0,
		ColorAttachment1,
		ColorAttachment2,
		ColorAttachment3,
		ColorAttachment4,
		ColorAttachment5,
		ColorAttachment6,
		ColorAttachment7,
		DepthAttachment,
		StencilAttachment,
		DepthStencilAttachment
	};

	enum class ImageFormat
	{
		RGB8,
		RGBA8,
		Depth24Stencil8
	};
}