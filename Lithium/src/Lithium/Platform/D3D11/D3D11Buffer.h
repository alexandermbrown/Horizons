#pragma once

#include "Lithium/Renderer/Buffer.h"

#include <d3d11.h>

namespace li
{
	class D3D11VertexBuffer : public VertexBuffer
	{
	public:
		D3D11VertexBuffer(uint32_t size, BufferUsage usage);
		D3D11VertexBuffer(float* vertices, uint32_t size, BufferUsage usage);
		virtual ~D3D11VertexBuffer();

		virtual void Bind() const override {};

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

		virtual void SetSubData(float* data, uint32_t size, uint32_t offset, bool discard) override;

		inline ID3D11Buffer* GetBuffer() { return m_Buffer; }

	private:
		BufferLayout m_Layout;
		ID3D11Buffer* m_Buffer = nullptr;
		uint32_t m_Size;

		ID3D11Device* m_DeviceHandle;
		ID3D11DeviceContext* m_ContextHandle;
	};

	class D3D11IndexBuffer : public IndexBuffer
	{
	public:
		D3D11IndexBuffer(uint32_t size, BufferUsage usage);
		D3D11IndexBuffer(uint32_t* indices, uint32_t count, BufferUsage usage);
		virtual ~D3D11IndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const {};

		virtual uint32_t GetCount() const { return m_Count; }

		virtual void SetSubData(uint32_t* data, uint32_t size, uint32_t offset, bool discard) override;

	private:
		ID3D11Buffer* m_Buffer = nullptr;
		uint32_t m_Size;
		uint32_t m_Count;

		ID3D11Device* m_DeviceHandle;
		ID3D11DeviceContext* m_ContextHandle;
	};
}