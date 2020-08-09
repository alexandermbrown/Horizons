#pragma once

#include "Lithium/Core/Core.h"
#include "RendererEnums.h"
#include "GraphicsContext.h"

#include <glm/glm.hpp>

namespace li
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL,
			Vulkan,
			D3D11,
			D3D12
		};

		virtual ~RendererAPI() = default;

		static void Create(RendererAPI::API api);

		inline static void ResizeView(int width, int height)
		{
			s_RendererAPI->ResizeViewImpl(width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColorImpl(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->ClearImpl();
		}

		inline static void DrawArrays(uint32_t vertexCount)
		{
			s_RendererAPI->DrawArraysImpl(vertexCount);
		}

		inline static void DrawIndexed(uint32_t indexCount)
		{
			s_RendererAPI->DrawIndexedImpl(indexCount);
		}

		inline static void DrawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount)
		{
			s_RendererAPI->DrawIndexedInstancedImpl(indexCount, instanceCount);
		}

		inline static void SetDepthTest(bool enabled)
		{
			s_RendererAPI->SetDepthTestImpl(enabled);
		}

		inline static void SetDrawMode(DrawMode mode)
		{
			s_RendererAPI->SetDrawModeImpl(mode);
		}

		inline static void SetContext(GraphicsContext* context)
		{
			s_RendererAPI->SetContextImpl(context);
		}

		inline static API GetAPI() { return s_API; }

	protected:
		virtual void ResizeViewImpl(int width, int height) = 0;
		virtual void SetClearColorImpl(const glm::vec4& color) = 0;
		virtual void ClearImpl() = 0;

		virtual void SetDepthTestImpl(bool enabled) = 0;

		virtual void DrawArraysImpl(uint32_t vertexCount) = 0;

		virtual void DrawIndexedImpl(uint32_t indexCount) = 0;
		virtual void DrawIndexedInstancedImpl(uint32_t indexCount, uint32_t instanceCount) = 0;

		virtual void SetDrawModeImpl(DrawMode mode) = 0;

		virtual void SetContextImpl(GraphicsContext* context) = 0;

	private:
		static API s_API;
		static Scope<RendererAPI> s_RendererAPI;
	};
}
