#include "lipch.h"
#include "D3D11Context.h"

#include "D3D11Core.h"
#include "glm/gtc/type_ptr.hpp"

#include <VersionHelpers.h>

using namespace Microsoft::WRL;

namespace Li
{
	D3D11Context::D3D11Context(HWND hwnd, int width, int height)
	{
		UINT device_flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef LI_DEBUG
		device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driver_types[] = {
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE
		};
		D3D_FEATURE_LEVEL feature_levels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0
		};

		HRESULT result = E_FAIL;
		D3D_FEATURE_LEVEL feature_level;
		for (D3D_DRIVER_TYPE driver : driver_types)
		{
			result = D3D11CreateDevice(nullptr, driver, nullptr, device_flags,
				feature_levels, sizeof(feature_levels) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION, &m_Device, &feature_level, &m_DeviceContext);

			if (SUCCEEDED(result))
				break;
		}

		if (FAILED(result))
			throw std::runtime_error("Failed to create Direct3D 11 Device.");

		if (feature_level >= D3D_FEATURE_LEVEL_11_1 && IsWindows7SP1OrGreater())
			InitSwapChain1(hwnd, width, height);
		else
			InitSwapChain(hwnd, width, height);

		if (!m_Device || !m_DeviceContext)
			throw std::runtime_error("Failed to create D3D11 device.");
		
		InitDepthStencil();
		InitBlendState();
		InitRasterState();
#ifdef LI_DEBUG
		InitDebug();
#endif
	}

	D3D11Context::~D3D11Context()
	{
		if (m_SwapChain)
			m_SwapChain->SetFullscreenState(false, NULL);
	}

	void D3D11Context::SwapBuffers()
	{
		m_SwapChain->Present(0, 0);
	}

	void D3D11Context::ResizeView(int width, int height)
	{
		m_DeviceContext->OMSetRenderTargets(0, 0, 0);

		m_RenderTargetView.Reset();
		m_DepthStencilView.Reset();
		m_DepthStencilBuffer.Reset();

		D3D11Call(m_SwapChain->ResizeBuffers(NumSwapChainBuffers, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

		ComPtr<ID3D11Texture2D> buffer;
		D3D11Call(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &buffer));

		D3D11Call(m_Device->CreateRenderTargetView(buffer.Get(), NULL, &m_RenderTargetView));

		// Set up the depth buffer.
		D3D11_TEXTURE2D_DESC depth_buffer_desc;
		ZeroMemory(&depth_buffer_desc, sizeof(depth_buffer_desc));

		depth_buffer_desc.Width = width;
		depth_buffer_desc.Height = height;
		depth_buffer_desc.MipLevels = 1;
		depth_buffer_desc.ArraySize = 1;
		depth_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depth_buffer_desc.SampleDesc.Count = 1; // Set MSAA here.
		depth_buffer_desc.SampleDesc.Quality = 0;
		depth_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		depth_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depth_buffer_desc.CPUAccessFlags = 0;
		depth_buffer_desc.MiscFlags = 0;

		D3D11Call(m_Device->CreateTexture2D(&depth_buffer_desc, NULL, &m_DepthStencilBuffer));

		D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
		ZeroMemory(&depth_stencil_view_desc, sizeof(depth_stencil_view_desc));

		depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depth_stencil_view_desc.Texture2D.MipSlice = 0;

		D3D11Call(m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &depth_stencil_view_desc, &m_DepthStencilView));

		m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());

		m_Viewport.Width = (float)width;
		m_Viewport.Height = (float)height;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
		m_Viewport.TopLeftX = 0.0f;
		m_Viewport.TopLeftY = 0.0f;
		m_DeviceContext->RSSetViewports(1, &m_Viewport);
	}

	void D3D11Context::BindDefaultRenderTarget()
	{
		m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
		m_DeviceContext->RSSetViewports(1, &m_Viewport);
	}

	void D3D11Context::Clear()
	{
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), glm::value_ptr(m_ClearColor));
		m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void D3D11Context::DrawArrays(uint32_t vertexCount)
	{
		m_DeviceContext->Draw(vertexCount, 0);
	}

	void D3D11Context::DrawIndexed(uint32_t indexCount)
	{
		m_DeviceContext->DrawIndexed(indexCount, 0, 0);
	}

	void D3D11Context::DrawIndexedInstanced(uint32_t indexCount, uint32_t instanceCount)
	{
		m_DeviceContext->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
	}

	void D3D11Context::SetDepthTest(bool enabled)
	{
	}

	void D3D11Context::SetDrawMode(DrawMode mode)
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
			LI_ASSERT(false, "Unknown draw mode!");
			break;
		}
	}

	void D3D11Context::InitSwapChain(HWND hwnd, int width, int height)
	{
		DXGI_SWAP_CHAIN_DESC swap_chain_desc = { 0 };
		swap_chain_desc.BufferDesc.Width = width;
		swap_chain_desc.BufferDesc.Height = height;
		swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 0;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
		swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swap_chain_desc.SampleDesc.Count = 1;
		swap_chain_desc.SampleDesc.Quality = 0;
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.BufferCount = 1;
		swap_chain_desc.OutputWindow = hwnd;
		swap_chain_desc.Windowed = true;

		if (IsWindows10OrGreater())
			swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		else if (IsWindows8OrGreater())
			swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		else
			swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		swap_chain_desc.Flags = 0;

		ComPtr<IDXGIDevice> dxgi_device;
		ComPtr<IDXGIAdapter> dxgi_adapter;
		ComPtr<IDXGIFactory> dxgi_factory;

		D3D11Call(m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgi_device));
		D3D11Call(dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgi_adapter));
		D3D11Call(dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgi_factory));
		D3D11Call(dxgi_factory->CreateSwapChain(m_Device.Get(), &swap_chain_desc, &m_SwapChain));
	}

	void D3D11Context::InitSwapChain1(HWND hwnd, int width, int height)
	{
		DXGI_SWAP_CHAIN_DESC1 swap_chain_desc;
		swap_chain_desc.Width = width;
		swap_chain_desc.Height = height;
		swap_chain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_desc.Stereo = false;
		swap_chain_desc.SampleDesc.Count = 1;
		swap_chain_desc.SampleDesc.Quality = 0;
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.BufferCount = NumSwapChainBuffers;
		swap_chain_desc.Scaling = DXGI_SCALING_STRETCH;
		if (IsWindows10OrGreater())
			swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		else if (IsWindows8OrGreater())
			swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		else
			swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swap_chain_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swap_chain_desc.Flags = 0;
		

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreen_desc;
		fullscreen_desc.RefreshRate.Numerator = 0;
		fullscreen_desc.RefreshRate.Denominator = 1;
		fullscreen_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		fullscreen_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		fullscreen_desc.Windowed = true;

		ComPtr<IDXGIDevice2> dxgi_device;
		ComPtr<IDXGIAdapter> dxgi_adapter;
		ComPtr<IDXGIFactory2> dxgi_factory;

		D3D11Call(m_Device->QueryInterface(__uuidof(IDXGIDevice2), (void**)&dxgi_device));
		D3D11Call(dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgi_adapter));
		D3D11Call(dxgi_adapter->GetParent(__uuidof(IDXGIFactory2), (void**)&dxgi_factory));

		ComPtr<IDXGISwapChain1> swap_chain1;
		D3D11Call(dxgi_factory->CreateSwapChainForHwnd(m_Device.Get(), hwnd, &swap_chain_desc, &fullscreen_desc, NULL, &swap_chain1));
		D3D11Call(swap_chain1.As(&m_SwapChain));
	}

	void D3D11Context::InitDepthStencil()
	{
		D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
		ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));
		
		depth_stencil_desc.DepthEnable = false;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;

		depth_stencil_desc.StencilEnable = false;
		depth_stencil_desc.StencilReadMask = 0xFF;
		depth_stencil_desc.StencilWriteMask = 0xFF;

		depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		D3D11Call(m_Device->CreateDepthStencilState(&depth_stencil_desc, &m_DepthStencilState));
		m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 1);
	}

	void D3D11Context::InitBlendState()
	{
		D3D11_BLEND_DESC blend_desc;
		ZeroMemory(&blend_desc, sizeof(D3D11_BLEND_DESC));
		blend_desc.AlphaToCoverageEnable = false;
		blend_desc.IndependentBlendEnable = false;

		blend_desc.RenderTarget[0].BlendEnable = true;
		blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		D3D11Call(m_Device->CreateBlendState(&blend_desc, &m_BlendState));

		m_DeviceContext->OMSetBlendState(m_BlendState.Get(), NULL, 0xffffffff);
	}

	void D3D11Context::InitRasterState()
	{
		// Setup the raster description which will determine how and what polygons will be drawn.
		D3D11_RASTERIZER_DESC raster_desc;
		raster_desc.FillMode = D3D11_FILL_SOLID;
		raster_desc.CullMode = D3D11_CULL_NONE;
		raster_desc.FrontCounterClockwise = true;
		raster_desc.DepthBias = 0;
		raster_desc.DepthBiasClamp = 0.0f;
		raster_desc.SlopeScaledDepthBias = 0.0f;
		raster_desc.DepthClipEnable = true;
		raster_desc.ScissorEnable = false;
		raster_desc.MultisampleEnable = false;
		raster_desc.AntialiasedLineEnable = false;
		// Create the rasterizer state from the description.
		D3D11Call(m_Device->CreateRasterizerState(&raster_desc, &m_RasterState));
		m_DeviceContext->RSSetState(m_RasterState.Get());
	}
#ifdef LI_DEBUG
	void D3D11Context::InitDebug()
	{
		ID3D11Debug* debug;
		ID3D11InfoQueue* info_queue;

		D3D11Call(m_Device->QueryInterface(__uuidof(ID3D11Debug), (void**)&debug));
		D3D11Call(debug->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&info_queue));

		info_queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
		info_queue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);

		D3D11_MESSAGE_ID hide[] = { D3D11_MESSAGE_ID_DEVICE_DRAW_SAMPLER_NOT_SET };
		D3D11_INFO_QUEUE_FILTER filter;
		memset(&filter, 0, sizeof(filter));
		filter.DenyList.NumIDs = _countof(hide);
		filter.DenyList.pIDList = hide;
		D3D11Call(info_queue->AddStorageFilterEntries(&filter));

		info_queue->Release();
		debug->Release();
	}
#endif
}
