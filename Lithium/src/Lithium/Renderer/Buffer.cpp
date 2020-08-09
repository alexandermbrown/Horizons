#include "lipch.h"
#include "Buffer.h"

#include "Lithium/Renderer/RendererAPI.h"

#ifdef LI_INCLUDE_OPENGL
#include "Lithium/Platform/OpenGL/OpenGLBuffer.h"
#endif
#ifdef LI_INCLUDE_D3D11
#include "Lithium/Platform/D3D11/D3D11Buffer.h"
#endif

namespace li
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, BufferUsage usage)
	{
		switch (RendererAPI::GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(size, usage);
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::API::D3D11:  return CreateRef<D3D11VertexBuffer>(size, usage);
#endif
		}

		LI_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size, BufferUsage usage)
	{
		switch (RendererAPI::GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(vertices, size, usage);
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::API::D3D11:  return CreateRef<D3D11VertexBuffer>(vertices, size, usage);
#endif
		}

		LI_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t size, BufferUsage usage)
	{
		switch (RendererAPI::GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLIndexBuffer>(size, usage);
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::API::D3D11:  return CreateRef<D3D11IndexBuffer>(size, usage);
#endif
		}

		LI_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (RendererAPI::GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLIndexBuffer>(indices, count);
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::API::D3D11:  return CreateRef<D3D11IndexBuffer>(indices, count);
#endif
		}

		LI_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	uint32_t BufferLayout::GetComponentCount() const
	{
		uint32_t count = 0;
		for (const BufferElement& element : m_Elements)
		{
			count += element.GetComponentCount();
		}
		return count;
	}
}