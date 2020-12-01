#include "lipch.h"
#include "D3D11Buffer.h"

#include "D3D11Core.h"
#include "D3D11Context.h"
#include "ConvertD3D11.h"

#include "Lithium/Core/Application.h"

namespace li
{
	D3D11VertexBuffer::D3D11VertexBuffer(uint32_t size, BufferUsage usage)
		: m_Size(size)
	{
		D3D11Context* context = (D3D11Context*)Application::Get()->GetWindow()->GetContext();
		m_DeviceHandle = context->GetDevice();
		m_ContextHandle = context->GetDeviceContext();

		if (usage == BufferUsage::DynamicDraw)
		{

			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.ByteWidth = size;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;

			D3D11Call( m_DeviceHandle->CreateBuffer(&bufferDesc, NULL, &m_Buffer) );
		}
		else LI_CORE_ASSERT(false, "Buffer without data must be dynamic.\n");
	}

	D3D11VertexBuffer::D3D11VertexBuffer(float* vertices, uint32_t size, BufferUsage usage)
		: m_Size(size)
	{
		D3D11Context* context = (D3D11Context*)Application::Get()->GetWindow()->GetContext();
		m_DeviceHandle = context->GetDevice();
		m_ContextHandle = context->GetDeviceContext();

		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = ConvertD3D11::BufferUsage(usage);
		bufferDesc.ByteWidth = size;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = usage == BufferUsage::DynamicDraw ? D3D11_CPU_ACCESS_WRITE : 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA bufferData;
		bufferData.pSysMem = vertices;
		bufferData.SysMemPitch = 0;
		bufferData.SysMemSlicePitch = 0;

		D3D11Call( m_DeviceHandle->CreateBuffer(&bufferDesc, &bufferData, &m_Buffer) );
	}

	D3D11VertexBuffer::~D3D11VertexBuffer()
	{
		m_Buffer->Release();
		m_Buffer = nullptr;
	}

	void D3D11VertexBuffer::SetSubData(float* data, uint32_t size, uint32_t offset, bool discard)
	{
		LI_CORE_ASSERT(size <= m_Size, "Buffer overflow.");

		D3D11_MAPPED_SUBRESOURCE resource;
		ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		// Disable GPU access to the vertex buffer data.
		D3D11Call( m_ContextHandle->Map(m_Buffer, 0, discard ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE, 0, &resource) );
		// Set the vertex data.
		memcpy((float*)resource.pData + offset, data, (size_t)size);
		// Reenable GPU access to the data.
		m_ContextHandle->Unmap(m_Buffer, 0);
	}


	D3D11IndexBuffer::D3D11IndexBuffer(uint32_t size, BufferUsage usage)
		: m_Size(size), m_Count()
	{
		D3D11Context* context = (D3D11Context*)Application::Get()->GetWindow()->GetContext();
		m_DeviceHandle = context->GetDevice();
		m_ContextHandle = context->GetDeviceContext();

		if (usage == BufferUsage::DynamicDraw)
		{
			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.ByteWidth = size;
			bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;

			D3D11Call( m_DeviceHandle->CreateBuffer(&bufferDesc, NULL, &m_Buffer) );
		}
		else LI_CORE_ASSERT(false, "Buffer without data must be dynamic.\n");
	}

	D3D11IndexBuffer::D3D11IndexBuffer(uint32_t* indices, uint32_t count, BufferUsage usage)
		: m_Size(count * sizeof(uint32_t)), m_Count(count)
	{
		D3D11Context* context = (D3D11Context*)Application::Get()->GetWindow()->GetContext();
		m_DeviceHandle = context->GetDevice();
		m_ContextHandle = context->GetDeviceContext();

		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = ConvertD3D11::BufferUsage(usage);
		bufferDesc.ByteWidth = sizeof(uint32_t) * count;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = (usage == BufferUsage::DynamicDraw) ? D3D11_CPU_ACCESS_WRITE : 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA bufferData;
		bufferData.pSysMem = indices;
		bufferData.SysMemPitch = 0;
		bufferData.SysMemSlicePitch = 0;

		D3D11Call(m_DeviceHandle->CreateBuffer(&bufferDesc, &bufferData, &m_Buffer) );
	}

	D3D11IndexBuffer::~D3D11IndexBuffer()
	{
		m_Buffer->Release();
		m_Buffer = nullptr;
	}

	void D3D11IndexBuffer::Bind() const
	{
		m_ContextHandle->IASetIndexBuffer(m_Buffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void D3D11IndexBuffer::SetSubData(uint32_t* data, uint32_t size, uint32_t offset, bool discard)
	{
		LI_CORE_ASSERT(size <= m_Size - offset, "Buffer overflow.");
		m_Count = offset / sizeof(uint32_t) + size / sizeof(uint32_t);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		// Disable GPU access to the vertex buffer data.
		D3D11Call(m_ContextHandle->Map(m_Buffer, 0, discard ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE, 0, &mappedResource));
		// Set the vertex data.
		memcpy((uint32_t*)mappedResource.pData + offset, data, (rsize_t)size);
		// Reenable GPU access to the data.
		m_ContextHandle->Unmap(m_Buffer, 0);
	}
}