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
			OpenGL
		};

		inline static void Init()
		{
			s_RendererAPI->InitImpl();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
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

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexedImpl(vertexArray);
		}

		inline static void DrawIndexedInstanced(
			const Ref<VertexArray>& vertexArray,
			uint32_t instanceCount,
			DrawMode mode = DrawMode::Triangles)
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
		virtual void SetViewportImpl(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColorImpl(const glm::vec4& color) = 0;
		virtual void ClearImpl() = 0;

		virtual void SetDepthTestImpl(bool enabled) = 0;

		virtual void DrawIndexedImpl(const Ref<VertexArray>& vertexArray) = 0;
		virtual void DrawIndexedInstancedImpl(
			const Ref<VertexArray>& vertexArray, 
			uint32_t instanceCount, 
			DrawMode mode) = 0;

	private:
		static API s_API;
		static Scope<RendererAPI> s_RendererAPI;
	};
}
