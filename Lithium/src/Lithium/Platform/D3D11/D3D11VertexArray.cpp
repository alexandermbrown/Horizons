#include "lipch.h"
#include "D3D11VertexArray.h"

#include "Lithium/Core/Application.h"
#include "D3D11Core.h"
#include "D3D11Context.h"
#include "D3D11Shader.h"

namespace li
{
	static DXGI_FORMAT ShaderDataTypeToD3D11(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return DXGI_FORMAT_R32_FLOAT;
		case ShaderDataType::Float2:   return DXGI_FORMAT_R32G32_FLOAT;
		case ShaderDataType::Float3:   return DXGI_FORMAT_R32G32B32_FLOAT;
		case ShaderDataType::Float4:   return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case ShaderDataType::Mat4:     return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case ShaderDataType::Int:      return DXGI_FORMAT_R32_SINT;
		case ShaderDataType::Int2:     return DXGI_FORMAT_R32G32_SINT;
		case ShaderDataType::Int3:     return DXGI_FORMAT_R32G32B32_SINT;
		case ShaderDataType::Int4:     return DXGI_FORMAT_R32G32B32A32_SINT;
			LI_CORE_ASSERT(false, "Bool not supported!");
			return DXGI_FORMAT_UNKNOWN;
		}
		LI_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return DXGI_FORMAT_UNKNOWN;
	}

	D3D11VertexArray::D3D11VertexArray()
		: m_D3DVertexBuffers(), m_VertexBufferStrides(), m_VertexBufferOffsets()
	{
		D3D11Context* context = (D3D11Context*)Application::Get()->GetWindow()->GetContext();
		m_DeviceHandle = context->GetDevice();
		m_ContextHandle = context->GetDeviceContext();
	}

	void D3D11VertexArray::Bind() const
	{
		LI_CORE_ASSERT(m_InputLayout && m_D3DVertexBuffers.size() == m_VertexBufferStrides.size() 
			&& m_D3DVertexBuffers.size() == m_VertexBufferOffsets.size(), "Did you remember to call Finalize()?");

		m_ContextHandle->IASetInputLayout(m_InputLayout);
		m_ContextHandle->IASetVertexBuffers(0, (UINT)m_D3DVertexBuffers.size(), m_D3DVertexBuffers.data(), m_VertexBufferStrides.data(), m_VertexBufferOffsets.data());
		m_IndexBuffer->Bind();
	}

	void D3D11VertexArray::Finalize(const Ref<Shader>& shader)
	{
		LI_CORE_ASSERT(m_InputLayout == nullptr && m_D3DVertexBuffers.size() == 0, "Cannot finalize more than once!");

		std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc = std::vector<D3D11_INPUT_ELEMENT_DESC>();

		for (const Ref<VertexBuffer>& vertexBuf : m_VertexBuffers)
		{
			D3D11VertexBuffer* vb = (D3D11VertexBuffer*)vertexBuf.get();
			m_D3DVertexBuffers.push_back(vb->GetBuffer());
			m_VertexBufferStrides.push_back(vb->GetLayout().GetStride());
			m_VertexBufferOffsets.push_back(0);


			for (const BufferElement& element : vb->GetLayout().GetElements())
			{
				if (element.Type == ShaderDataType::Mat4)
				{
					for (int row = 0; row < 4; row++)
					{
						D3D11_INPUT_ELEMENT_DESC desc;
						desc.SemanticName = element.Name.c_str();
						desc.SemanticIndex = row;
						desc.Format = ShaderDataTypeToD3D11(element.Type);
						desc.InputSlot = element.Divisor;
						desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
						desc.InputSlotClass = element.Divisor > 0 ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
						desc.InstanceDataStepRate = element.Divisor;
						inputDesc.push_back(desc);
					}
				}
				else
				{
					D3D11_INPUT_ELEMENT_DESC desc;
					desc.SemanticName = element.Name.c_str();
					desc.SemanticIndex = 0;
					desc.Format = ShaderDataTypeToD3D11(element.Type);
					desc.InputSlot = element.Divisor;
					desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
					desc.InputSlotClass = element.Divisor > 0 ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
					desc.InstanceDataStepRate = element.Divisor;
					inputDesc.push_back(desc);
				}
			}
		}

		LI_CORE_ASSERT(inputDesc.size() < 32, "Layout too large!");

		Ref<D3D11Shader> d3dShader = std::static_pointer_cast<D3D11Shader>(shader);
		D3D11Call( m_DeviceHandle->CreateInputLayout(inputDesc.data(), (uint32_t)inputDesc.size(), d3dShader->GetVSBufferData(), d3dShader->GetVSBufferSize(), &m_InputLayout) );
	}
}