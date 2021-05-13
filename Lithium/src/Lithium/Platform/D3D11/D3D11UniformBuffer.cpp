#include "lipch.h"
#include "D3D11UniformBuffer.h"

#include "D3D11Core.h"
#include "D3D11Context.h"
#include "Lithium/Core/Application.h"

namespace Li
{
	D3D11UniformBuffer::D3D11UniformBuffer(uint32_t bindingSlot, uint32_t size)
		: m_Slot(bindingSlot), m_Size(size)
	{
		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = m_Size;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11Context* context = (D3D11Context*)Application::Get().GetWindow().GetContext();
		m_ContextHandle = context->GetDeviceContext();

		D3D11Call(context->GetDevice()->CreateBuffer(&desc, NULL, &m_Buffer));
	}

	void D3D11UniformBuffer::Bind(ShaderType type) const
	{
		switch (type)
		{
		case ShaderType::Vertex:
			m_ContextHandle->VSSetConstantBuffers(m_Slot, 1, m_Buffer.GetAddressOf());
			break;
		case ShaderType::Geometry:
			m_ContextHandle->GSSetConstantBuffers(m_Slot, 1, m_Buffer.GetAddressOf());
			break;
		case ShaderType::Fragment:
			m_ContextHandle->PSSetConstantBuffers(m_Slot, 1, m_Buffer.GetAddressOf());
			break;
		}
	}

	void D3D11UniformBuffer::SetData(const void* data)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		// Disable GPU access to the vertex buffer data.
		D3D11Call(m_ContextHandle->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
		// Set the vertex data.
		memcpy(mappedResource.pData, data, m_Size);
		// Reenable GPU access to the data.
		m_ContextHandle->Unmap(m_Buffer.Get(), 0);
	}
}
