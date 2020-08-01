#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/VertexArray.h"

#include <glm/glm.hpp>

namespace li
{
	class RendererAPI
	{
	public:
		enum class API
		{
			OpenGL,
			Vulkan,
			D3D11,
			D3D12
		};

		virtual ~RendererAPI() = default;

		inline static void Init()
		{
			s_RendererAPI->InitImpl();
		}

		inline static void SetViewport(int x, int y, int width, int height)
		{
			s_RendererAPI->SetViewportImpl(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColorImpl(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->ClearImpl();
		}

		inline static void DrawArrays(const Ref<VertexArray>& vertexArray, uint32_t count, DrawMode mode)
		{
			s_RendererAPI->DrawArraysImpl(vertexArray, count, mode);
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexedImpl(vertexArray);
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count, DrawMode mode)
		{
			s_RendererAPI->DrawIndexedImpl(vertexArray, count, mode);
		}

		inline static void DrawIndexedInstanced(const Ref<VertexArray>& vertexArray,
			uint32_t instanceCount, DrawMode mode = DrawMode::Triangles)
		{
			s_RendererAPI->DrawIndexedInstancedImpl(vertexArray, instanceCount, mode);
		}

		inline static void SetDepthTest(bool enabled)
		{
			s_RendererAPI->SetDepthTestImpl(enabled);
		}

		inline static API GetAPI() { return s_API; }
		static Scope<RendererAPI> Create();

	protected:
		virtual void InitImpl() = 0;
		virtual void SetViewportImpl(int x, int y, int width, int height) = 0;
		virtual void SetClearColorImpl(const glm::vec4& color) = 0;
		virtual void ClearImpl() = 0;

		virtual void SetDepthTestImpl(bool enabled) = 0;

		virtual void DrawArraysImpl(const Ref<VertexArray>& vertexArray, uint32_t count, DrawMode mode) = 0;

		virtual void DrawIndexedImpl(const Ref<VertexArray>& vertexArray) = 0;
		virtual void DrawIndexedImpl(const Ref<VertexArray>& vertexArray, uint32_t count, DrawMode mode) = 0;
		virtual void DrawIndexedInstancedImpl(const Ref<VertexArray>& vertexArray, 
			uint32_t instanceCount, DrawMode mode) = 0;

	private:
		static API s_API;
		static Scope<RendererAPI> s_RendererAPI;
	};
}
