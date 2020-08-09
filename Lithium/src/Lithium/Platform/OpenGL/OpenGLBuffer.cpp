#include "lipch.h"
#include "Lithium/Platform/OpenGL/OpenGLBuffer.h"
#include "Lithium/Platform/OpenGL/ConvertOpenGL.h"

#include "OpenGLCore.h"

#include "glad/glad.h"

namespace li {

	/////////////////////////////////////////////////////////////////////////////
	// VertexBuffer /////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size, BufferUsage usage)
	{
		GLCall( glCreateBuffers(1, &m_RendererID) );
		GLCall( glBindBuffer(GL_ARRAY_BUFFER, m_RendererID) );

		GLCall( glBufferData(GL_ARRAY_BUFFER, size, nullptr, ConvertOpenGL::BufferUsage(usage)) );
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size, BufferUsage usage)
	{
		GLCall( glCreateBuffers(1, &m_RendererID) );
		GLCall( glBindBuffer(GL_ARRAY_BUFFER, m_RendererID) );

		GLCall( glBufferData(GL_ARRAY_BUFFER, size, vertices, ConvertOpenGL::BufferUsage(usage)) );
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		GLCall( glDeleteBuffers(1, &m_RendererID) );
	}

	void OpenGLVertexBuffer::Bind() const
	{
		GLCall( glBindBuffer(GL_ARRAY_BUFFER, m_RendererID) );
	}

	void OpenGLVertexBuffer::SetSubData(float* data, uint32_t size, uint32_t offset, bool discard, BufferTarget target)
	{
		GLCall( glBindBuffer(ConvertOpenGL::BufferTarget(target), m_RendererID) );
		GLCall( glBufferSubData(ConvertOpenGL::BufferTarget(target), offset, static_cast<GLsizeiptr>(size), data) );
	}

	/////////////////////////////////////////////////////////////////////////////
	// IndexBuffer //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t size, BufferUsage usage)
		: m_Count(0)
	{
		GLCall( glCreateBuffers(1, &m_RendererID) );
		GLCall( glBindBuffer(GL_ARRAY_BUFFER, m_RendererID) );

		GLCall( glBufferData(GL_ARRAY_BUFFER, size, nullptr, ConvertOpenGL::BufferUsage(usage)) );
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		GLCall( glCreateBuffers(1, &m_RendererID) );
		GLCall( glBindBuffer(GL_ARRAY_BUFFER, m_RendererID) );
		GLCall( glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW) );
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		GLCall( glDeleteBuffers(1, &m_RendererID) );
	}

	void OpenGLIndexBuffer::Bind() const
	{
		GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID) );
	}

	void OpenGLIndexBuffer::SetSubData(uint32_t* data, uint32_t size, uint32_t offset, bool discard, BufferTarget target)
	{
		m_Count = size / sizeof(uint32_t);
		GLCall( glBindBuffer(ConvertOpenGL::BufferTarget(target), m_RendererID) );
		GLCall( glBufferSubData(ConvertOpenGL::BufferTarget(target), offset, static_cast<GLsizeiptr>(size), data) );
	}

}
