#include "lipch.h"
#include "OpenGLShaderBuffer.h"

#include "OpenGLCore.h"

#include "glad/glad.h"

namespace Li
{
	OpenGLShaderBuffer::OpenGLShaderBuffer(uint32_t size)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);

		glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_STATIC_DRAW);
	}

	OpenGLShaderBuffer::~OpenGLShaderBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	void OpenGLShaderBuffer::Bind(uint32_t slot) const
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, m_RendererID);
	}

	void OpenGLShaderBuffer::SetSubData(uint32_t* data, uint32_t size, uint32_t offset, bool discard)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);

		GLbitfield access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT;
		if (discard) access |= GL_MAP_INVALIDATE_BUFFER_BIT;

		void* dest = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, offset, size, access);
		LI_CORE_ASSERT(dest, "Error mapping buffer.");
		memcpy(dest, data, size);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}
}
