#include "lipch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "OpenGLCore.h"

namespace li 
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return GL_FLOAT;
			case ShaderDataType::Float2:   return GL_FLOAT;
			case ShaderDataType::Float3:   return GL_FLOAT;
			case ShaderDataType::Float4:   return GL_FLOAT;
			case ShaderDataType::Mat4:     return GL_FLOAT;
			case ShaderDataType::Int:      return GL_INT;
			case ShaderDataType::Int2:     return GL_INT;
			case ShaderDataType::Int3:     return GL_INT;
			case ShaderDataType::Int4:     return GL_INT;
		}

		LI_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		GLCall( glGenVertexArrays(1, &m_RendererID) );
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		GLCall( glDeleteVertexArrays(1, &m_RendererID) );
	}

	void OpenGLVertexArray::Bind() const
	{
		GLCall( glBindVertexArray(m_RendererID) );
		if (m_IndexBuffer) m_IndexBuffer->Bind();
	}

	void OpenGLVertexArray::Unbind() const
	{
		GLCall( glBindVertexArray(0) );
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		LI_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		vertexBuffer->Bind();
		GLCall( glBindVertexArray(m_RendererID) );

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			if (element.Type == ShaderDataType::Mat4)
			{
				for (int i = 0; i < 4; i++)
				{
					GLCall( glEnableVertexAttribArray(element.Location + i) );
					GLCall( glVertexAttribPointer(element.Location + i,
						element.GetComponentCount() / 4,
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.Offset + i * sizeof(glm::vec4))) );
					
					if (element.Divisor > 0) {
						GLCall( glVertexAttribDivisor(element.Location + i, element.Divisor) );
					}
				}
			}
			else
			{
				GLCall( glEnableVertexAttribArray(element.Location) );
				GLCall( glVertexAttribPointer(element.Location,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)(element.Offset)) );

				if (element.Divisor > 0) {
					GLCall( glVertexAttribDivisor(element.Location, element.Divisor) );
				}
			}
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		GLCall( glBindVertexArray(m_RendererID) );
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}
