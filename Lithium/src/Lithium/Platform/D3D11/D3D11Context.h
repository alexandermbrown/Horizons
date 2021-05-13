#pragma once

#include "Lithium/Renderer/GraphicsContext.h"

#include <d3d11_2.h>
#include <wrl/client.h>

#include "glm/glm.hpp"

namespace Li
{
	class D3D11Context : public GraphicsContext
	{
	public:
		D3D11Context(HWND hwnd, int width, int height);
		~D3D11Context();

		virtual void BindDefaultRenderTarget() override;
		virtual void Clear() override;
		virtual void DrawArrays(uint32_t vertex_count) override;
		virtual void DrawIndexed(uint32_t index_count) override;
		virtual void DrawIndexedInstanced(uint32_t index_count, uint32_t instance_count) override;
		virtual void DispatchCompute(uint32_t num_groups_x, uint32_t num_groups_y, uint32_t num_groups_z) override;
		virtual void ShaderStorageBarrier() override {};
		virtual void ResizeView(int width, int height) override;
		virtual void SetClearColor(const glm::vec4& color) override { m_ClearColor = color; }
		virtual void SetDepthTest(bool enabled) override;
		virtual void SetDrawMode(DrawMode mode) override;
		virtual void SwapBuffers() override;

		inline const Microsoft::WRL::ComPtr<ID3D11Device>& GetDevice() { return m_Device; }
		inline const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetDeviceContext() { return m_DeviceContext; }
		inline const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& GetDepthStencilView() { return m_DepthStencilView; }

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

		Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_DepthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RasterState;
		Microsoft::WRL::ComPtr<ID3D11BlendState> m_BlendState;

		D3D11_VIEWPORT m_Viewport;

		glm::vec4 m_ClearColor;
	};
}
