#include "lipch.h"
#include "OpenGLUniformBuffer.h"

#include "OpenGLCore.h"

#include "glad/glad.h"

namespace Li
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(const std::string& name, uint32_t bindingSlot, const UniformBufferLayout& layout)
		: m_Name(name), m_Slot(bindingSlot), m_Elements()
	{
		m_DataSize = layout.GetByteSize();
		m_GlslData = new uint8_t[m_DataSize];

		for (const UniformBufferElement& element : layout)
		{
			m_Elements[element.Name] = element;
		}

		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);

		glBufferData(GL_UNIFORM_BUFFER, m_DataSize, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
		delete[] m_GlslData;
	}

	void OpenGLUniformBuffer::SetInt(const std::string& name, int value)
	{
		const UniformBufferElement& element = m_Elements[name];
		LI_CORE_ASSERT(element.Type == ShaderDataType::Int, "Incorrect uniform type!");

#ifdef LI_ENABLE_ASSERTS
		m_Changed = true;
#endif
		LI_CORE_VERIFY(!memcpy_s(m_GlslData + element.Offset, m_DataSize, &value, sizeof(value)), "Failed to copy memory!");
	}

	void OpenGLUniformBuffer::SetFloat(const std::string& name, float value)
	{
		const UniformBufferElement& element = m_Elements[name];
		LI_CORE_ASSERT(element.Type == ShaderDataType::Float, "Incorrect uniform type!");

#ifdef LI_ENABLE_ASSERTS
		m_Changed = true;
#endif
		LI_CORE_VERIFY(!memcpy_s(m_GlslData + element.Offset, m_DataSize, &value, sizeof(value)), "Failed to copy memory!");
	}

	void OpenGLUniformBuffer::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		const UniformBufferElement& element = m_Elements[name];
		LI_CORE_ASSERT(element.Type == ShaderDataType::Float3, "Incorrect uniform type!");

#ifdef LI_ENABLE_ASSERTS
		m_Changed = true;
#endif
		LI_CORE_VERIFY(!memcpy_s(m_GlslData + element.Offset, m_DataSize, &value, sizeof(value)), "Failed to copy memory!");
	}

	void OpenGLUniformBuffer::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		const UniformBufferElement& element = m_Elements[name];
		LI_CORE_ASSERT(element.Type == ShaderDataType::Float4, "Incorrect uniform type!");

#ifdef LI_ENABLE_ASSERTS
		m_Changed = true;
#endif
		LI_CORE_VERIFY(!memcpy_s(m_GlslData + element.Offset, m_DataSize, &value, sizeof(value)), "Failed to copy memory!");
	}

	void OpenGLUniformBuffer::SetMat4(const std::string& name, const glm::mat4& value)
	{
		const UniformBufferElement& element = m_Elements[name];
		LI_CORE_ASSERT(element.Type == ShaderDataType::Mat4, "Incorrect uniform type!");

#ifdef LI_ENABLE_ASSERTS
		m_Changed = true;
#endif
		
		LI_CORE_VERIFY(!memcpy_s(m_GlslData + element.Offset, m_DataSize, &value, sizeof(value)), "Failed to copy memory!");
	}

	void OpenGLUniformBuffer::UploadData()
	{
		LI_CORE_ASSERT(m_Changed, "Data not changed");
		
		constexpr GLbitfield access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		void* dest = glMapBufferRange(GL_UNIFORM_BUFFER, 0, (GLsizeiptr)m_DataSize, access);
		LI_CORE_ASSERT(dest, "Failed to map buffer.");
		memcpy(dest, m_GlslData, m_DataSize);
		glUnmapBuffer(GL_UNIFORM_BUFFER);
	}

	void OpenGLUniformBuffer::BindToSlot() const
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, m_Slot, m_RendererID);
	}

	ShaderType OpenGLUniformBuffer::GetShaderType() const
	{
		Log::CoreWarn("OpenGL uniform buffers do not have a shader type.");
		return ShaderType::None;
	}
}