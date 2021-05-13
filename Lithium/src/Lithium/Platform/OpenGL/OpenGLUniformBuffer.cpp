#include "lipch.h"
#include "OpenGLUniformBuffer.h"

#include "OpenGLCore.h"

#include "glad/glad.h"

namespace Li
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t slot, uint32_t size)
		: m_Slot(slot), m_Size(size)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);

		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLUniformBuffer::BindToSlot() const
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, m_Slot, m_RendererID);
	}

	void OpenGLUniformBuffer::SetData(const void* data)
	{
		constexpr GLbitfield access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		void* dest = glMapBufferRange(GL_UNIFORM_BUFFER, 0, (GLsizeiptr)m_Size, access);
		LI_CORE_ASSERT(dest, "Failed to map buffer.");
		memcpy(dest, data, m_Size);
		glUnmapBuffer(GL_UNIFORM_BUFFER);
	}
}
