#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/VertexArray.h"
#include "Lithium/Renderer/Shader.h"

#include "D3D11Buffer.h"

#include <vector>
#include <d3d11.h>

namespace Li
{
	class D3D11VertexArray : public VertexArray
	{
	public:
		D3D11VertexArray();
		virtual ~D3D11VertexArray() = default;

		virtual void Bind() const override;
		virtual void Unbind() const override {};

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override { m_VertexBuffers.push_back(vertexBuffer); };
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override { m_IndexBuffer = indexBuffer; };

		virtual void Finalize(const Ref<Shader>& shader) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
	private:
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;

		ID3D11InputLayout* m_InputLayout = nullptr;

		std::vector<ID3D11Buffer*> m_D3DVertexBuffers;

		std::vector<unsigned int> m_VertexBufferStrides;
		std::vector<unsigned int> m_VertexBufferOffsets;

		ID3D11Device* m_DeviceHandle;
		ID3D11DeviceContext* m_ContextHandle;
	};
}
