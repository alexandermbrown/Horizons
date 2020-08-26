#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/RendererAPI.h"

namespace li
{
	class OpenGLRendererAPI : public RendererAPI
	{
	protected:
		virtual void ResizeViewImpl(int width, int height) override;

		virtual void SetClearColorImpl(const glm::vec4& color) override;
		virtual void ClearImpl() override;

		virtual void SetDepthTestImpl(bool enabled) override;

		virtual void DrawArraysImpl(uint32_t vertexCount) override;

		virtual void DrawIndexedImpl(uint32_t indexCount) override;
		virtual void DrawIndexedInstancedImpl(uint32_t indexCount, uint32_t instanceCount) override;

		virtual void BindDefaultRenderTargetImpl() override;

		virtual void SetDrawModeImpl(DrawMode mode) override { m_DrawMode = mode; }

		virtual void SetContextImpl(GraphicsContext* context) override {};

	private:

		bool m_DepthTest = false;
		DrawMode m_DrawMode = DrawMode::Triangles;

		int m_Width;
		int m_Height;
	};
}