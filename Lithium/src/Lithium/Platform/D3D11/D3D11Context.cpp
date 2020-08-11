#include "lipch.h"
#include "D3D11Context.h"

#include "D3D11Core.h"
#include "glm/gtc/type_ptr.hpp"

namespace li
{
	D3D11Context::D3D11Context(HWND hwnd, int width, int height)
	{
		const D3D_FEATURE_LEVEL levels[7] = {
		D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1
		};

		UINT createDeviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef LI_DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D11Call(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags,
			levels, sizeof(levels) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION, &m_Device, NULL, &m_DeviceContext));

		InitSwapChain(hwnd, width, height);
		InitDepthStencil();
		InitBlendState();
		InitRasterState();
		Resize(width, height);
	}

	D3D11Context::~D3D11Context()
	{
		if (m_SwapChain)
		{
			m_SwapChain->SetFullscreenState(false, NULL);
		}

		LI_D3D_RELEASE(m_RasterState);
		LI_D3D_RELEASE(m_DepthStencilView);
		LI_D3D_RELEASE(m_DepthStencilState);
		LI_D3D_RELEASE(m_DepthStencilBuffer);
		LI_D3D_RELEASE(m_RenderTargetView);
		LI_D3D_RELEASE(m_DeviceContext);
		LI_D3D_RELEASE(m_Device);
		LI_D3D_RELEASE(m_SwapChain);
	}

	void D3D11Context::SwapBuffers()
	{
		m_SwapChain->Present(1, 0);
	}

	void D3D11Context::Resize(int width, int height)
	{
		m_DeviceContext->OMSetRenderTargets(0, 0, 0);

		LI_D3D_RELEASE(m_RenderTargetView);
		LI_D3D_RELEASE(m_DepthStencilView);
		LI_D3D_RELEASE(m_DepthStencilBuffer);

		D3D11Call(m_SwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

		ID3D11Texture2D* buffer;
		D3D11Call(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer));

		D3D11Call(m_Device->CreateRenderTargetView(buffer, NULL, &m_RenderTargetView));
		buffer->Release();


		// Set up the description of the depth buffer.
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		depthBufferDesc.Width = width;
		depthBufferDesc.Height = height;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1; // Set MSAA here.
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		D3D11Call(m_Device->CreateTexture2D(&depthBufferDesc, NULL, &m_DepthStencilBuffer));

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		D3D11Call(m_Device->CreateDepthStencilView(m_DepthStencilBuffer, &depthStencilViewDesc, &m_DepthStencilView));

		m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

		D3D11_VIEWPORT viewport;
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		m_DeviceContext->RSSetViewports(1, &viewport);
	}

	void D3D11Context::Clear()
	{
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, glm::value_ptr(m_ClearColor));
		m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void D3D11Context::InitSwapChain(HWND hwnd, int width, int height)
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		// Single back buffer.
		swapChainDesc.BufferCount = 1;

		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// Turn off multisampling.
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = 0;

		IDXGIDevice* dxgiDevice = nullptr;
		IDXGIAdapter* dxgiAdapter = nullptr;
		IDXGIFactory* dxgiFactory = nullptr;

		D3D11Call(m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));
		D3D11Call(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));
		D3D11Call(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));
		D3D11Call(dxgiFactory->CreateSwapChain(m_Device, &swapChainDesc, &m_SwapChain));
	}

	void D3D11Context::InitDepthStencil()
	{
		// TODO: make sure that these are the correct settings,
		// add an option to disable testing.


		// Initialize the description of the stencil state.
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		// Set up the description of the stencil state.
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the depth stencil state.
		D3D11Call(m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState));

		// Set the depth stencil state.
		m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);
	}

	void D3D11Context::InitBlendState()
	{
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;

		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		D3D11Call(m_Device->CreateBlendState(&blendDesc, &m_BlendState));

		m_DeviceContext->OMSetBlendState(m_BlendState, NULL, 0xffffffff);
	}

	void D3D11Context::InitRasterState()
	{
		// Setup the raster description which will determine how and what polygons will be drawn.
		D3D11_RASTERIZER_DESC rasterDesc;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.FrontCounterClockwise = true;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.SlopeScaledDepthBias = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.ScissorEnable = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.AntialiasedLineEnable = false;
		// Create the rasterizer state from the description.
		D3D11Call(m_Device->CreateRasterizerState(&rasterDesc, &m_RasterState));
		m_DeviceContext->RSSetState(m_RasterState);
	}
}

