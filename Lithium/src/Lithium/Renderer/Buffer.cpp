#include "lipch.h"
#include "Buffer.h"

#include "Lithium/Renderer/Renderer.h"
#include "Lithium/Renderer/RendererAPI.h"
#include "Lithium/Platform/OpenGL/OpenGLBuffer.h"

namespace li
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, BufferUsage usage)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(size, usage);
		}

		LI_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size, BufferUsage usage)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(vertices, size, usage);
		}

		LI_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t size, BufferUsage usage)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLIndexBuffer>(size, usage);
		}

		LI_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLIndexBuffer>(indices, count);
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