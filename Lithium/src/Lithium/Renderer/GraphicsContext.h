#pragma once

#include "glm/glm.hpp"
#include "Lithium/Renderer/RendererEnums.h"

namespace li
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void BindDefaultRenderTarget() = 0;
		virtual void Clear() = 0;
		virtual void DrawArrays(uint32_t vertexCount) = 0;
		virtual void DrawIndexed(uint32_t indexCount) = 0;
		virtual void DrawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount) = 0;
		virtual void ResizeView(int width, int height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void SetDepthTest(bool enabled) = 0;
		virtual void SetDrawMode(DrawMode mode) = 0;
		virtual void SwapBuffers() = 0;
	};
}
