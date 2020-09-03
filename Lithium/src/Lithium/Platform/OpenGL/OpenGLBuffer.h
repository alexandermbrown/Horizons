#pragma once

#include "Lithium/Renderer/Buffer.h"

namespace li
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size, BufferUsage usage);
		OpenGLVertexBuffer(float* vertices, uint32_t size, BufferUsage usage);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

		virtual void SetSubData(float* data, uint32_t size, uint32_t offset, bool discard) override;

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t size, BufferUsage usage);
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count, BufferUsage usage);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;

		virtual uint32_t GetCount() const override { return m_Count; }

		virtual void SetSubData(uint32_t* data, uint32_t size, uint32_t offset, bool discard) override;

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

}