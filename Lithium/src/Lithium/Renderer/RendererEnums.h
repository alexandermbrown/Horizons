#pragma once

namespace Li
{
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat4, Int, Int2, Int3, Int4
	};

	enum class ShaderType
	{
		None = 0,
		Vertex,
		Geometry,
		Fragment
	};

	enum class BufferUsage
	{
		Error = 0,
		StaticDraw,
		DynamicDraw
	};

	enum class DrawMode
	{
		Error = 0,
		Points,
		Lines,
		LineStrip,
		Triangles,
		TriangleStrip
	};

	enum class WrapType
	{
		Error = 0,
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder
	};

	enum class FilterType
	{
		Error = 0,
		Nearest,
		Linear,
		NearestMipmapNearest,
		LinearMipmapNearest,
		NearestMipmapLinear
	};

	//enum class FramebufferTarget
	//{
	//	Framebuffer = 0,
	//	ReadFramebuffer,
	//	DrawFramebuffer
	//};

	//enum class FramebufferAttachment
	//{
	//	ColorAttachment0 = 0,
	//	ColorAttachment1,
	//	ColorAttachment2,
	//	ColorAttachment3,
	//	ColorAttachment4,
	//	ColorAttachment5,
	//	ColorAttachment6,
	//	ColorAttachment7,
	//	DepthAttachment,
	//	StencilAttachment,
	//	DepthStencilAttachment
	//};

	//enum class ImageFormat
	//{
	//	RGB = 0,
	//	RGBA,
	//	DepthStencil
	//};

	//enum class InternalImageFormat
	//{
	//	RGB8 = 0,
	//	RGBA8,
	//	Depth24Stencil8
	//};
}