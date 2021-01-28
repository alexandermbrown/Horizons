#include "lipch.h"
#include "D3D11Framebuffer.h"

#include "D3D11Core.h"
#include "D3D11Context.h"
#include "Lithium/Core/Application.h"

#include "glm/gtc/type_ptr.hpp"

namespace Li
{
	D3D11Framebuffer::D3D11Framebuffer(int width, int height)
		: m_Size(width, height)
	{
		D3D11Context* context = (D3D11Context*)Application::Get().GetWindow().GetContext();
		m_DeviceHandle = context->GetDevice();
		m_ContextHandle = context->GetDeviceContext();

		m_Texture = MakeRef<D3D11Texture2D>(width, height, 4, nullptr,
			WrapType::ClampToEdge, WrapType::ClampToEdge,
			FilterType::Linear, FilterType::Nearest, false, true);

		CreateBuffers(width, height);
	}

	void D3D11Framebuffer::Bind() const
	{
		m_ContextHandle->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
		m_ContextHandle->RSSetViewports(1, &m_Viewport);
	}

	void D3D11Framebuffer::Clear() const
	{
		m_ContextHandle->ClearRenderTargetView(m_RenderTargetView.Get(), glm::value_ptr(m_ClearColor));
		m_ContextHandle->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void D3D11Framebuffer::Resize(int width, int height)
	{
		m_RenderTargetView.Reset();

		m_Size = { width, height };
		m_Texture->Resize(width, height);
		CreateBuffers(width, height);
	}

	void D3D11Framebuffer::CreateBuffers(int width, int height)
	{
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc;
		ZeroMemory(&renderTargetDesc, sizeof(renderTargetDesc));
		renderTargetDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetDesc.Texture2D.MipSlice = 0;

		D3D11Call( m_DeviceHandle->CreateRenderTargetView(m_Texture->GetTexture(), &renderTargetDesc, &m_RenderTargetView) );

		// Set up the description of the depth buffer.
		D3D11_TEXTURE2D_DESC depth_buffer_desc;
		ZeroMemory(&depth_buffer_desc, sizeof(depth_buffer_desc));
		depth_buffer_desc.Width = width;
		depth_buffer_desc.Height = height;
		depth_buffer_desc.MipLevels = 1;
		depth_buffer_desc.ArraySize = 1;
		depth_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depth_buffer_desc.SampleDesc.Count = 1;
		depth_buffer_desc.SampleDesc.Quality = 0;
		depth_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		depth_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depth_buffer_desc.CPUAccessFlags = 0;
		depth_buffer_desc.MiscFlags = 0;

		D3D11Call( m_DeviceHandle->CreateTexture2D(&depth_buffer_desc, NULL, &m_DepthStencilBuffer) );

		D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
		ZeroMemory(&depth_stencil_view_desc, sizeof(depth_stencil_view_desc));
		depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depth_stencil_view_desc.Texture2D.MipSlice = 0;

		D3D11Call( m_DeviceHandle->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &depth_stencil_view_desc, &m_DepthStencilView) );

		m_Viewport.Width = (float)width;
		m_Viewport.Height = (float)height;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
		m_Viewport.TopLeftX = 0.0f;
		m_Viewport.TopLeftY = 0.0f;
	}
}
