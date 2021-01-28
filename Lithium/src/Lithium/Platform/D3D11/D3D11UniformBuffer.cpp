#include "lipch.h"
#include "D3D11UniformBuffer.h"

#include "D3D11Core.h"
#include "D3D11Context.h"
#include "Lithium/Core/Application.h"

namespace Li
{
	D3D11UniformBuffer::D3D11UniformBuffer(const std::string& name, uint32_t bindingSlot, ShaderType shaderType, const UniformBufferLayout& layout)
		: m_Name(name), m_Slot(bindingSlot), m_Type(shaderType)
	{
		m_DataSize = layout.GetByteSize();
		m_GlslData = new uint8_t[m_DataSize];

		for (const UniformBufferElement& element : layout)
		{
			m_Elements[element.Name] = element;
		}

		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = layout.GetByteSize();
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11Context* context = (D3D11Context*)Application::Get().GetWindow().GetContext();
		m_ContextHandle = context->GetDeviceContext();

		D3D11Call(context->GetDevice()->CreateBuffer(&desc, NULL, &m_Buffer));
	}

	D3D11UniformBuffer::~D3D11UniformBuffer()
	{
		delete[] m_GlslData;
	}

	void D3D11UniformBuffer::SetInt(const std::string& name, int value)
	{
		const UniformBufferElement& element = m_Elements[name];
		LI_CORE_ASSERT(element.Type == ShaderDataType::Int, "Incorrect uniform type!");

#ifdef LI_ENABLE_ASSERTS
		m_Changed = true;
#endif
		LI_CORE_VERIFY(!memcpy_s(m_GlslData + element.Offset, m_DataSize, &value, sizeof(value)), "Failed to copy memory!");
	}

	void D3D11UniformBuffer::SetFloat(const std::string& name, float value)
	{
		const UniformBufferElement& element = m_Elements[name];
		LI_CORE_ASSERT(element.Type == ShaderDataType::Float, "Incorrect uniform type!");

#ifdef LI_ENABLE_ASSERTS
		m_Changed = true;
#endif
		LI_CORE_VERIFY(!memcpy_s(m_GlslData + element.Offset, m_DataSize, &value, sizeof(value)), "Failed to copy memory!");
	}

	void D3D11UniformBuffer::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		const UniformBufferElement& element = m_Elements[name];
		LI_CORE_ASSERT(element.Type == ShaderDataType::Float3, "Incorrect uniform type!");

#ifdef LI_ENABLE_ASSERTS
		m_Changed = true;
#endif
		LI_CORE_VERIFY(!memcpy_s(m_GlslData + element.Offset, m_DataSize, &value, sizeof(value)), "Failed to copy memory!");
	}

	void D3D11UniformBuffer::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		const UniformBufferElement& element = m_Elements[name];
		LI_CORE_ASSERT(element.Type == ShaderDataType::Float4, "Incorrect uniform type!");

#ifdef LI_ENABLE_ASSERTS
		m_Changed = true;
#endif
		LI_CORE_VERIFY(!memcpy_s(m_GlslData + element.Offset, m_DataSize, &value, sizeof(value)), "Failed to copy memory!");
	}

	void D3D11UniformBuffer::SetMat4(const std::string& name, const glm::mat4& value)
	{
		const UniformBufferElement& element = m_Elements[name];
		LI_CORE_ASSERT(element.Type == ShaderDataType::Mat4, "Incorrect uniform type!");

#ifdef LI_ENABLE_ASSERTS
		m_Changed = true;
#endif

		LI_CORE_VERIFY(!memcpy_s(m_GlslData + element.Offset, m_DataSize, &value, sizeof(value)), "Failed to copy memory!");
	}

	void D3D11UniformBuffer::UploadData()
	{
		LI_CORE_ASSERT(m_Changed, "Data not changed");

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		// Disable GPU access to the vertex buffer data.
		D3D11Call(m_ContextHandle->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
		// Set the vertex data.
		memcpy(mappedResource.pData, m_GlslData, m_DataSize);
		// Reenable GPU access to the data.
		m_ContextHandle->Unmap(m_Buffer.Get(), 0);
	}

	void D3D11UniformBuffer::Bind() const
	{
		switch (m_Type)
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
}
