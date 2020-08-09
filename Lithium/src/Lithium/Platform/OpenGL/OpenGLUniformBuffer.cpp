#include "lipch.h"
#include "OpenGLUniformBuffer.h"

#include "OpenGLCore.h"

#include "glad/glad.h"

namespace li
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

		GLCall( glCreateBuffers(1, &m_RendererID) );
		GLCall( glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID) );

		GLCall( glBufferData(GL_UNIFORM_BUFFER, m_DataSize, nullptr, GL_DYNAMIC_DRAW) );
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		GLCall( glDeleteBuffers(1, &m_RendererID) );
		delete[] m_GlslData;
	}

	void OpenGLUniformBuffer::SetInt(const std::string& name, int value)
	{
		const UniformBufferElement& element = m_Elements[name];
		LI_CORE_ASSERT(element.Type == ShaderDataType::Int, "Incorrect uniform type!");

#ifdef LI_ENABLE_ASSERTS
		m_Changed = true;
#endif
		LI_CORE_RUN_ASSERT(!memcpy_s(m_GlslData + element.Offset, m_DataSize, &value, sizeof(value)), "Failed to copy memory!");
	}

	void OpenGLUniformBuffer::SetFloat(const std::string& name, float value)
	{
		const UniformBufferElement& element = m_Elements[name];
		LI_CORE_ASSERT(element.Type == ShaderDataType::Float, "Incorrect uniform type!");

#ifdef LI_ENABLE_ASSERTS
		m_Changed = true;
#endif
		LI_CORE_RUN_ASSERT(!memcpy_s(m_GlslData + element.Offset, m_DataSize, &value, sizeof(value)), "Failed to copy memory!");
	}

	void OpenGLUniformBuffer::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		const UniformBufferElement& element = m_Elements[name];
		LI_CORE_ASSERT(element.Type == ShaderDataType::Float3, "Incorrect uniform type!");

#ifdef LI_ENABLE_ASSERTS
		m_Changed = true;
#endif
		LI_CORE_RUN_ASSERT(!memcpy_s(m_GlslData + element.Offset, m_DataSize, &value, sizeof(value)), "Failed to copy memory!");
	}

	void OpenGLUniformBuffer::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		const UniformBufferElement& element = m_Elements[name];
		LI_CORE_ASSERT(element.Type == ShaderDataType::Float4, "Incorrect uniform type!");

#ifdef LI_ENABLE_ASSERTS
		m_Changed = true;
#endif
		LI_CORE_RUN_ASSERT(!memcpy_s(m_GlslData + element.Offset, m_DataSize, &value, sizeof(value)), "Failed to copy memory!");
	}

	void OpenGLUniformBuffer::SetMat4(const std::string& name, const glm::mat4& value)
	{
		const UniformBufferElement& element = m_Elements[name];
		LI_CORE_ASSERT(element.Type == ShaderDataType::Mat4, "Incorrect uniform type!");

#ifdef LI_ENABLE_ASSERTS
		m_Changed = true;
#endif
		
		LI_CORE_RUN_ASSERT(!memcpy_s(m_GlslData + element.Offset, m_DataSize, &value, sizeof(value)), "Failed to copy memory!");
	}

	void OpenGLUniformBuffer::UploadData()
	{
		LI_CORE_ASSERT(m_Changed, "Data not changed");

		// TODO: Orphan buffer or use multiple buffers.
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID));
		GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, (GLsizeiptr)m_DataSize, m_GlslData));
	}

	void OpenGLUniformBuffer::BindToSlot() const
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, m_Slot, m_RendererID);
	}

	ShaderType OpenGLUniformBuffer::GetShaderType() const
	{
		LI_CORE_WARN("OpenGL uniform buffers do not have a shader type.");
		return ShaderType::None;
	}
}