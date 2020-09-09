#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/UniformBuffer.h"

#include <d3d11.h>
#include <unordered_map>
#include <string>

namespace li
{
	class D3D11UniformBuffer : public UniformBuffer
	{
	public:
		D3D11UniformBuffer(const std::string& name, uint32_t bindingSlot, ShaderType shaderType, const UniformBufferLayout& layout);
		virtual ~D3D11UniformBuffer();

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		virtual void UploadData() override;
		virtual void BindToSlot() const override {};
		virtual void Bind() const override;

		virtual inline const std::string& GetName() const override { return m_Name; }
		virtual ShaderType GetShaderType() const override { return m_Type; }
		virtual inline uint32_t GetSlot() const override { return m_Slot; }

	private:
		std::string m_Name;
		ShaderType m_Type;

#ifdef LI_ENABLE_ASSERTS
		bool m_Changed = false;
#endif

		uint8_t* m_GlslData;
		uint32_t m_DataSize;

		std::unordered_map<std::string, UniformBufferElement> m_Elements;

		ID3D11Buffer* m_Buffer;
		uint32_t m_Slot;

		ID3D11DeviceContext* m_ContextHandle;
	};
}