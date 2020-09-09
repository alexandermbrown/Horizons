#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/RendererEnums.h"

#include "glm/glm.hpp"
#include <string>
#include <vector>
#include <unordered_map>

namespace li
{
	struct UniformBufferElement
	{
		std::string Name;
		ShaderDataType Type;

		uint32_t Offset;
	};

	class UniformBufferLayout
	{
	public:

		UniformBufferLayout(const std::initializer_list<UniformBufferElement>& elements);

		inline uint32_t GetByteSize() const { return (uint32_t)m_Size; }
		inline uint32_t GetElementCount() const { return (uint32_t)m_Elements.size(); }
		inline const std::vector<UniformBufferElement>& GetElements() const { return m_Elements; }

		std::vector<UniformBufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<UniformBufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<UniformBufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<UniformBufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		std::vector<UniformBufferElement> m_Elements;
		uint32_t m_Size;
	};

	class UniformBuffer
	{
	public:

		virtual ~UniformBuffer() = default;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		virtual void UploadData() = 0;
		virtual void BindToSlot() const = 0;
		virtual void Bind() const = 0;

		virtual const std::string& GetName() const = 0;
		virtual ShaderType GetShaderType() const = 0;
		virtual uint32_t GetSlot() const = 0;

		static Ref<UniformBuffer> Create(const std::string& name, uint32_t bindingSlot, ShaderType shaderType, const UniformBufferLayout& layout);
	};
}
