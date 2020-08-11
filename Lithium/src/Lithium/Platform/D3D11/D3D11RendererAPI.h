#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/RendererAPI.h"
#include "Lithium/Renderer/RendererEnums.h"

#include "D3D11Context.h"

#include "glm/glm.hpp"

#include <d3d11.h>

namespace li
{
	class D3D11RendererAPI : public RendererAPI
	{
	public:

		virtual void ResizeViewImpl(int width, int height) override;

		virtual void SetClearColorImpl(const glm::vec4& color) override;
		virtual void ClearImpl() override;

		virtual void SetDepthTestImpl(bool enabled) override;

		virtual void DrawArraysImpl(uint32_t vertexCount) override;

		virtual void DrawIndexedImpl(uint32_t indexCount) override;
		virtual void DrawIndexedInstancedImpl(uint32_t indexCount, uint32_t instanceCount) override;

		virtual void SetDrawModeImpl(DrawMode mode) override;

		virtual void SetContextImpl(GraphicsContext* context)
		{
			m_Context = (D3D11Context*)context;
			m_DeviceContext = m_Context->GetDeviceContext();
		}

	private:

		D3D11Context* m_Context = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;
	};
}
