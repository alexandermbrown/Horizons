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

	D3D11Framebuffer::~D3D11Framebuffer()
	{
		m_RenderTargetView->Release();
		m_RenderTargetView = nullptr;
	}

	void D3D11Framebuffer::Bind() const
	{
		m_ContextHandle->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
		m_ContextHandle->RSSetViewports(1, &m_Viewport);
	}

	void D3D11Framebuffer::Clear() const
	{
		m_ContextHandle->ClearRenderTargetView(m_RenderTargetView, glm::value_ptr(m_ClearColor));
		m_ContextHandle->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void D3D11Framebuffer::Resize(int width, int height)
	{
		m_RenderTargetView->Release();

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
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
		depthBufferDesc.Width = width;
		depthBufferDesc.Height = height;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		D3D11Call( m_DeviceHandle->CreateTexture2D(&depthBufferDesc, NULL, &m_DepthStencilBuffer) );

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		D3D11Call( m_DeviceHandle->CreateDepthStencilView(m_DepthStencilBuffer, &depthStencilViewDesc, &m_DepthStencilView) );

		m_Viewport.Width = (float)width;
		m_Viewport.Height = (float)height;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
		m_Viewport.TopLeftX = 0.0f;
		m_Viewport.TopLeftY = 0.0f;
	}
}