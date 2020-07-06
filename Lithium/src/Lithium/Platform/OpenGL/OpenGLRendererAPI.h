#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/RendererAPI.h"

namespace li
{
	class OpenGLRendererAPI : public RendererAPI
	{
	protected:
		virtual void InitImpl() override;
		virtual void SetViewportImpl(int x, int y, int width, int height) override;

		virtual void SetClearColorImpl(const glm::vec4& color) override;
		virtual void ClearImpl() override;

		virtual void SetDepthTestImpl(bool enabled) override;

		virtual void DrawArraysImpl(const Ref<VertexArray>& vertexArray, uint32_t count, DrawMode mode) override;

		virtual void DrawIndexedImpl(const Ref<VertexArray>& vertexArray) override;
		virtual void DrawIndexedImpl(const Ref<VertexArray>& vertexArray, uint32_t count, DrawMode mode) override;
		virtual void DrawIndexedInstancedImpl(
			const Ref<VertexArray>& vertexArray,
			uint32_t instanceCount,
			DrawMode mode) override;

	private:

		bool m_DepthTest = false;
	};
}