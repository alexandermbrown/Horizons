#include "lipch.h"
#include "D3D11Texture.h"

#include "D3D11Core.h"
#include "D3D11Context.h"
#include "ConvertD3D11.h"

#include "Lithium/Core/Application.h"

#include "stb_image.h"

namespace li
{
	D3D11Texture2D::D3D11Texture2D(int width, int height, void* data, WrapType wrapS, WrapType wrapT, FilterType minFilter, FilterType magFilter, bool renderTarget)
		: m_Width(width), m_Height(height), m_IsRenderTarget(renderTarget)
	{
		D3D11Context* context = (D3D11Context*)Application::Get()->GetWindow()->GetContext();
		m_DeviceHandle = context->GetDevice();
		m_ContextHandle = context->GetDeviceContext();

		D3D11_TEXTURE2D_DESC textureDesc;
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		if (renderTarget) textureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		if (data)
		{
			D3D11_SUBRESOURCE_DATA bufferData;
			bufferData.pSysMem = data;
			bufferData.SysMemPitch = 4 * width;
			bufferData.SysMemSlicePitch = 4 * width * height;

			D3D11Call( m_DeviceHandle->CreateTexture2D(&textureDesc, &bufferData, &m_Texture) );
		}
		else
		{
			D3D11Call( m_DeviceHandle->CreateTexture2D(&textureDesc, NULL, &m_Texture) );
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
		ZeroMemory(&viewDesc, sizeof(viewDesc));
		viewDesc.Format = textureDesc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MostDetailedMip = 0;
		viewDesc.Texture2D.MipLevels = textureDesc.MipLevels;

		D3D11Call(m_DeviceHandle->CreateShaderResourceView(m_Texture, &viewDesc, &m_ResourceView));


		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = CalculateFilter(minFilter, magFilter);
		samplerDesc.AddressU = ConvertD3D11::WrapType(wrapS);
		samplerDesc.AddressV = ConvertD3D11::WrapType(wrapT);
		samplerDesc.AddressW = ConvertD3D11::WrapType(wrapT);
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		D3D11Call( m_DeviceHandle->CreateSamplerState(&samplerDesc, &m_SamplerState) );
	}

	D3D11Texture2D::D3D11Texture2D(const std::string& path, WrapType wrapS, WrapType wrapT, FilterType minFilter, FilterType magFilter)
		: m_IsRenderTarget(false)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = nullptr;
		data = stbi_load(path.c_str(), &width, &height, &channels, 4);

		LI_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		D3D11Context* context = (D3D11Context*)Application::Get()->GetWindow()->GetContext();
		m_DeviceHandle = context->GetDevice();
		m_ContextHandle = context->GetDeviceContext();

		D3D11_TEXTURE2D_DESC textureDesc;
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA bufferData;
		bufferData.pSysMem = data;
		bufferData.SysMemPitch = 4 * width;
		bufferData.SysMemSlicePitch = 4 * width * height;

		D3D11Call(m_DeviceHandle->CreateTexture2D(&textureDesc, &bufferData, &m_Texture));

		stbi_image_free(data);

		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
		viewDesc.Format = textureDesc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MostDetailedMip = 0;
		viewDesc.Texture2D.MipLevels = textureDesc.MipLevels;

		D3D11Call(m_DeviceHandle->CreateShaderResourceView(m_Texture, &viewDesc, &m_ResourceView));

		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = CalculateFilter(minFilter, magFilter);
		samplerDesc.AddressU = ConvertD3D11::WrapType(wrapS);
		samplerDesc.AddressV = ConvertD3D11::WrapType(wrapT);
		samplerDesc.AddressW = ConvertD3D11::WrapType(wrapT);
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		D3D11Call(m_DeviceHandle->CreateSamplerState(&samplerDesc, &m_SamplerState));
	}

	D3D11Texture2D::D3D11Texture2D(size_t imageSize, const uint8_t* rawData, WrapType wrapS, WrapType wrapT, FilterType minFilter, FilterType magFilter)
		: m_IsRenderTarget(false)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = nullptr;
		data = stbi_load_from_memory(rawData, (int)imageSize, &width, &height, &channels, 4);

		LI_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		D3D11Context* context = (D3D11Context*)Application::Get()->GetWindow()->GetContext();
		m_DeviceHandle = context->GetDevice();
		m_ContextHandle = context->GetDeviceContext();

		D3D11_TEXTURE2D_DESC textureDesc;
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA bufferData;
		bufferData.pSysMem = data;
		bufferData.SysMemPitch = 4 * width;
		bufferData.SysMemSlicePitch = 4 * width * height;

		D3D11Call(m_DeviceHandle->CreateTexture2D(&textureDesc, &bufferData, &m_Texture));

		stbi_image_free(data);

		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
		viewDesc.Format = textureDesc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MostDetailedMip = 0;
		viewDesc.Texture2D.MipLevels = textureDesc.MipLevels;

		D3D11Call(m_DeviceHandle->CreateShaderResourceView(m_Texture, &viewDesc, &m_ResourceView));

		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = CalculateFilter(minFilter, magFilter);
		samplerDesc.AddressU = ConvertD3D11::WrapType(wrapS);
		samplerDesc.AddressV = ConvertD3D11::WrapType(wrapT);
		samplerDesc.AddressW = ConvertD3D11::WrapType(wrapT);
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		D3D11Call(m_DeviceHandle->CreateSamplerState(&samplerDesc, &m_SamplerState));
	}

	D3D11Texture2D::~D3D11Texture2D()
	{
		m_ResourceView->Release();
		m_Texture->Release();
		m_SamplerState->Release();
	}

	void D3D11Texture2D::Resize(int width, int height)
	{
		m_Texture->Release();
		m_ResourceView->Release();

		D3D11_TEXTURE2D_DESC textureDesc;
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		if (m_IsRenderTarget) textureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		D3D11Call(m_DeviceHandle->CreateTexture2D(&textureDesc, NULL, &m_Texture));

		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
		ZeroMemory(&viewDesc, sizeof(viewDesc));
		viewDesc.Format = textureDesc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MostDetailedMip = 0;
		viewDesc.Texture2D.MipLevels = textureDesc.MipLevels;

		D3D11Call(m_DeviceHandle->CreateShaderResourceView(m_Texture, &viewDesc, &m_ResourceView));
	}

	void D3D11Texture2D::Bind(uint32_t slot) const
	{
		m_ContextHandle->PSSetShaderResources(slot, 1, &m_ResourceView);
		m_ContextHandle->PSSetSamplers(slot, 1, &m_SamplerState);
	}

	D3D11_FILTER D3D11Texture2D::CalculateFilter(FilterType minFilter, FilterType magFilter)
	{
		if (minFilter == FilterType::Nearest)
		{
			switch (magFilter)
			{
			case FilterType::Nearest:
				return D3D11_FILTER_MIN_MAG_MIP_POINT;
			case FilterType::Linear:
				return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
			}
		}
		else if (minFilter == FilterType::Linear)
		{
			switch (magFilter)
			{
			case FilterType::Nearest:
				return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
			case FilterType::Linear:
				return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			}
		}
		
		LI_CORE_ASSERT(false, "Unsupported filter type!");
		return D3D11_FILTER_MIN_MAG_MIP_POINT;
	}
}