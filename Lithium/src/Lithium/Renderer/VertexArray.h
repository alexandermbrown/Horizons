#pragma once

#include "Lithium/Core/Memory.h"
#include "Lithium/Renderer/Buffer.h"
#include "Lithium/Renderer/Shader.h"

#include <memory>

namespace Li 
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual void Finalize(const std::shared_ptr<Shader>& shader) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static Ref<VertexArray> Create();
	};
}
