#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/UniformBuffer.h"

#include <unordered_map>
#include <string>

namespace Li
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(const std::string& name, uint32_t bindingSlot, const UniformBufferLayout& layout);
		virtual ~OpenGLUniformBuffer();

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		virtual void UploadData() override;
		virtual void BindToSlot() const override;
		virtual void Bind() const override {};

		virtual inline const std::string& GetName() const override { return m_Name; }
		virtual ShaderType GetShaderType() const override;
		virtual inline uint32_t GetSlot() const override { return m_Slot; }

	private:
		std::string m_Name;

#ifdef LI_ENABLE_ASSERTS
		bool m_Changed = false;
#endif

		uint8_t* m_GlslData;
		uint32_t m_DataSize;

		std::unordered_map<std::string, UniformBufferElement> m_Elements;

		uint32_t m_RendererID;
		uint32_t m_Slot;
	};
}