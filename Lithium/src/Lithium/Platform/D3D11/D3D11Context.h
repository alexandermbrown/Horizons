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

		void SwapBuffers();

		void Resize(int width, int height);

		inline void SetClearColor(const glm::vec4& color) { m_ClearColor = color; }
		void Clear();
		void BindDefaultRenderTarget();


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
