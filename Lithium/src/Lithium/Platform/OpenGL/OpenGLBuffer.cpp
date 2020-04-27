#include "lipch.h"
#include "Lithium/Platform/OpenGL/OpenGLBuffer.h"
#include "Lithium/Platform/OpenGL/ConvertOpenGL.h"

#include <glad/glad.h>

namespace li {

	/////////////////////////////////////////////////////////////////////////////
	// VertexBuffer /////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size, BufferUsage usage)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

		glBufferData(GL_ARRAY_BUFFER, size, nullptr, ConvertOpenGL::BufferUsage(usage));
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size, BufferUsage usage)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

		glBufferData(GL_ARRAY_BUFFER, size, vertices, ConvertOpenGL::BufferUsage(usage));
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(float* data, uint32_t size, uint32_t offset, BufferUsage usage, BufferTarget target)
	{
		glBindBuffer(ConvertOpenGL::BufferTarget(target), m_RendererID);
		glBufferData(m_RendererID, size, data, ConvertOpenGL::BufferUsage(usage));
	}

	void OpenGLVertexBuffer::SetSubData(float* data, uint32_t size, uint32_t offset, BufferTarget target)
	{
		glBindBuffer(ConvertOpenGL::BufferTarget(target), m_RendererID);
		glBufferSubData(ConvertOpenGL::BufferTarget(target), offset, size, data);
	}

	/////////////////////////////////////////////////////////////////////////////
	// IndexBuffer //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t size, BufferUsage usage)
		: m_Count(0)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

		glBufferData(GL_ARRAY_BUFFER, size, nullptr, ConvertOpenGL::BufferUsage(usage));
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void OpenGLIndexBuffer::SetSubData(uint32_t* data, uint32_t size, uint32_t offset, BufferTarget target)
	{
		m_Count = size / sizeof(uint32_t);
		glBindBuffer(ConvertOpenGL::BufferTarget(target), m_RendererID);
		glBufferSubData(ConvertOpenGL::BufferTarget(target), offset, size, data);
	}

}
