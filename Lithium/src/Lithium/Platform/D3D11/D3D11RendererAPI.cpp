#include "lipch.h"
#include "D3D11RendererAPI.h"


namespace li
{
	void D3D11RendererAPI::ResizeViewImpl(int width, int height)
	{
		m_Context->Resize(width, height);
	}

	void D3D11RendererAPI::SetClearColorImpl(const glm::vec4& color)
	{
		m_Context->SetClearColor(color);
	}

	void D3D11RendererAPI::ClearImpl()
	{
		m_Context->Clear();
	}

	void D3D11RendererAPI::SetDepthTestImpl(bool enabled)
	{
		// TODO.
		// LI_CORE_ASSERT(false, "Not implemented!");
	}

	void D3D11RendererAPI::DrawArraysImpl(uint32_t vertexCount)
	{
		m_DeviceContext->Draw(vertexCount, 0);
	}

	void D3D11RendererAPI::DrawIndexedImpl(uint32_t indexCount)
	{
		m_DeviceContext->DrawIndexed(indexCount, 0, 0);
	}

	void D3D11RendererAPI::DrawIndexedInstancedImpl(uint32_t indexCount, uint32_t instanceCount)
	{
		m_DeviceContext->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
	}

	void D3D11RendererAPI::BindDefaultRenderTargetImpl()
	{
		m_Context->BindDefaultRenderTarget();
	}

	void D3D11RendererAPI::SetDrawModeImpl(DrawMode mode)
	{
		switch (mode)
		{
		case DrawMode::Points:
			m_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
			break;
		case DrawMode::Lines:
			m_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
			break;
		case DrawMode::LineStrip:
			m_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
			break;
		case DrawMode::Triangles:
			m_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			break;
		case DrawMode::TriangleStrip:
			m_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			break;
		default:
			break;
		}
	}
}