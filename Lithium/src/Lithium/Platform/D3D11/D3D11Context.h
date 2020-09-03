#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/GraphicsContext.h"

#include <d3d11.h>

#include "glm/glm.hpp"

namespace li
{
	class D3D11Context : public GraphicsContext
	{
	public:
		D3D11Context(HWND hwnd, int width, int height);
		~D3D11Context();

		virtual void BindDefaultRenderTarget() override;
		virtual void Clear() override;
		virtual void DrawArrays(uint32_t vertexCount) override;
		virtual void DrawIndexed(uint32_t indexCount) override;
		virtual void DrawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount) override;
		virtual void ResizeView(int width, int height) override;
		virtual void SetClearColor(const glm::vec4& color) override { m_ClearColor = color; }
		virtual void SetDepthTest(bool enabled) override;
		virtual void SetDrawMode(DrawMode mode) override;
		virtual void SwapBuffers() override;

		ID3D11Device* GetDevice() { return m_Device; }
		ID3D11DeviceContext* GetDeviceContext() { return m_DeviceContext; }
		ID3D11DepthStencilView* GetDepthStencilView() { return m_DepthStencilView; }

	private:
		constexpr static int NumSwapChainBuffers = 2;

		void InitSwapChain(HWND hwnd, int width, int height);
		void InitSwapChain1(HWND hwnd, int width, int height);
		void InitDepthStencil();
		void InitBlendState();
		void InitRasterState();
#ifdef LI_DEBUG
		void InitDebug();
#endif

		IDXGISwapChain* m_SwapChain = nullptr;

		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_DeviceContext = nullptr;
		ID3D11RenderTargetView* m_RenderTargetView = nullptr;
		ID3D11Texture2D* m_DepthStencilBuffer = nullptr;
		ID3D11DepthStencilView* m_DepthStencilView = nullptr;
		ID3D11DepthStencilState* m_DepthStencilState = nullptr;
		ID3D11RasterizerState* m_RasterState = nullptr;
		ID3D11BlendState* m_BlendState = nullptr;

		D3D11_VIEWPORT m_Viewport;

		glm::vec4 m_ClearColor;
	};
}
