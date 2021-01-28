#include "lipch.h"
#include "D3D11Texture.h"

#include "D3D11Core.h"
#include "D3D11Context.h"
#include "ConvertD3D11.h"

#include "Lithium/Core/Application.h"

#include "stb_image.h"

namespace Li
{
	D3D11Texture2D::D3D11Texture2D(int width, int height, int channels, void* data,
		WrapType wrap_s, WrapType wrap_t, FilterType min_filter, FilterType mag_filter, bool dynamic, bool render_target)
		: m_Width(width), m_Height(height), m_Channels(channels), m_Dynamic(dynamic), m_RenderTarget(render_target)
	{
		D3D11Context* context = (D3D11Context*)Application::Get().GetWindow().GetContext();
		m_DeviceHandle = context->GetDevice();
		m_ContextHandle = context->GetDeviceContext();

		D3D11_TEXTURE2D_DESC texture_desc;
		texture_desc.Width = width;
		texture_desc.Height = height;
		texture_desc.MipLevels = 1;
		texture_desc.ArraySize = 1;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		if (render_target) texture_desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		texture_desc.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE: 0;
		texture_desc.MiscFlags = 0;
		texture_desc.Format = ConvertD3D11::TextureFormat(m_Channels);

		if (data)
		{
			D3D11_SUBRESOURCE_DATA buffer_data;
			buffer_data.pSysMem = data;
			buffer_data.SysMemPitch = channels * width;
			buffer_data.SysMemSlicePitch = channels * width * height;
			D3D11Call( m_DeviceHandle->CreateTexture2D(&texture_desc, &buffer_data, &m_Texture) );
		}
		else D3D11Call( m_DeviceHandle->CreateTexture2D(&texture_desc, NULL, &m_Texture) );

		D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
		ZeroMemory(&view_desc, sizeof(view_desc));
		view_desc.Format = texture_desc.Format;
		view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		view_desc.Texture2D.MostDetailedMip = 0;
		view_desc.Texture2D.MipLevels = texture_desc.MipLevels;

		D3D11Call(m_DeviceHandle->CreateShaderResourceView(m_Texture.Get(), &view_desc, &m_ResourceView));

		D3D11_SAMPLER_DESC sampler_desc;
		sampler_desc.Filter = CalculateFilter(min_filter, mag_filter);
		sampler_desc.AddressU = ConvertD3D11::WrapType(wrap_s);
		sampler_desc.AddressV = ConvertD3D11::WrapType(wrap_t);
		sampler_desc.AddressW = ConvertD3D11::WrapType(wrap_t);
		sampler_desc.MipLODBias = 0.0f;
		sampler_desc.MaxAnisotropy = 1;
		sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
		D3D11Call( m_DeviceHandle->CreateSamplerState(&sampler_desc, &m_SamplerState) );
	}

	D3D11Texture2D::D3D11Texture2D(const std::string& path, int desired_channels,
		WrapType wrap_s, WrapType wrap_t, FilterType min_filter, FilterType mag_filter)
		: m_Dynamic(false), m_RenderTarget(false)
	{
		stbi_set_flip_vertically_on_load(1);

		if (desired_channels < 1 || desired_channels == 3 || desired_channels > 4)
		{
			Log::CoreError("Texture cannot have {} channels, defaulting to 4.", desired_channels);
			desired_channels = 4;
		}
		m_Channels = desired_channels;

		stbi_uc* data = stbi_load(path.c_str(), &m_Width, &m_Height, nullptr, desired_channels);

		LI_CORE_ASSERT(data, "Failed to load image!");

		D3D11Context* context = (D3D11Context*)Application::Get().GetWindow().GetContext();
		m_DeviceHandle = context->GetDevice();
		m_ContextHandle = context->GetDeviceContext();

		D3D11_TEXTURE2D_DESC texture_desc;
		texture_desc.Width = m_Width;
		texture_desc.Height = m_Height;
		texture_desc.MipLevels = 1;
		texture_desc.ArraySize = 1;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texture_desc.CPUAccessFlags = 0;
		texture_desc.MiscFlags = 0;
		texture_desc.Format = ConvertD3D11::TextureFormat(m_Channels);

		D3D11_SUBRESOURCE_DATA bufferData;
		bufferData.pSysMem = data;
		bufferData.SysMemPitch = m_Channels * m_Width;
		bufferData.SysMemSlicePitch = m_Channels * m_Width * m_Height;
		D3D11Call(m_DeviceHandle->CreateTexture2D(&texture_desc, &bufferData, &m_Texture));

		stbi_image_free(data);

		D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
		view_desc.Format = texture_desc.Format;
		view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		view_desc.Texture2D.MostDetailedMip = 0;
		view_desc.Texture2D.MipLevels = texture_desc.MipLevels;
		D3D11Call(m_DeviceHandle->CreateShaderResourceView(m_Texture.Get(), &view_desc, &m_ResourceView));

		D3D11_SAMPLER_DESC sampler_desc;
		sampler_desc.Filter = CalculateFilter(min_filter, mag_filter);
		sampler_desc.AddressU = ConvertD3D11::WrapType(wrap_s);
		sampler_desc.AddressV = ConvertD3D11::WrapType(wrap_t);
		sampler_desc.AddressW = ConvertD3D11::WrapType(wrap_t);
		sampler_desc.MipLODBias = 0.0f;
		sampler_desc.MaxAnisotropy = 1;
		sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
		D3D11Call(m_DeviceHandle->CreateSamplerState(&sampler_desc, &m_SamplerState));
	}

	D3D11Texture2D::D3D11Texture2D(size_t image_size, const uint8_t* encoded_data, int desired_channels,
		WrapType wrap_s, WrapType wrap_t, FilterType min_filter, FilterType mag_filter)
		: m_Dynamic(false), m_RenderTarget(false)
	{
		stbi_set_flip_vertically_on_load(1);
		int channels_in_file;

		stbi_uc* data = nullptr;
		data = stbi_load_from_memory(encoded_data, (int)image_size, &m_Width, &m_Height, &channels_in_file, 4);
		m_Channels = 4;

		LI_CORE_ASSERT(data, "Failed to load image!");

		D3D11Context* context = (D3D11Context*)Application::Get().GetWindow().GetContext();
		m_DeviceHandle = context->GetDevice();
		m_ContextHandle = context->GetDeviceContext();

		D3D11_TEXTURE2D_DESC texture_desc;
		texture_desc.Width = m_Width;
		texture_desc.Height = m_Height;
		texture_desc.MipLevels = 1;
		texture_desc.ArraySize = 1;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texture_desc.CPUAccessFlags = 0;
		texture_desc.MiscFlags = 0;
		texture_desc.Format = ConvertD3D11::TextureFormat(m_Channels);

		D3D11_SUBRESOURCE_DATA buffer_data;
		buffer_data.pSysMem = data;
		buffer_data.SysMemPitch = m_Channels * m_Width;
		buffer_data.SysMemSlicePitch = m_Channels * m_Width * m_Height;

		D3D11Call(m_DeviceHandle->CreateTexture2D(&texture_desc, &buffer_data, &m_Texture));

		stbi_image_free(data);

		D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
		view_desc.Format = texture_desc.Format;
		view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		view_desc.Texture2D.MostDetailedMip = 0;
		view_desc.Texture2D.MipLevels = texture_desc.MipLevels;

		D3D11Call(m_DeviceHandle->CreateShaderResourceView(m_Texture.Get(), &view_desc, &m_ResourceView));

		D3D11_SAMPLER_DESC sampler_desc;
		sampler_desc.Filter = CalculateFilter(min_filter, mag_filter);
		sampler_desc.AddressU = ConvertD3D11::WrapType(wrap_s);
		sampler_desc.AddressV = ConvertD3D11::WrapType(wrap_t);
		sampler_desc.AddressW = ConvertD3D11::WrapType(wrap_t);
		sampler_desc.MipLODBias = 0.0f;
		sampler_desc.MaxAnisotropy = 1;
		sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

		D3D11Call(m_DeviceHandle->CreateSamplerState(&sampler_desc, &m_SamplerState));
	}

	void D3D11Texture2D::Resize(int width, int height)
	{
		m_Texture.Reset();
		m_ResourceView.Reset();

		D3D11_TEXTURE2D_DESC texture_desc;
		texture_desc.Width = width;
		texture_desc.Height = height;
		texture_desc.MipLevels = 1;
		texture_desc.ArraySize = 1;
		texture_desc.Format = ConvertD3D11::TextureFormat(m_Channels);
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		texture_desc.CPUAccessFlags = 0;
		texture_desc.MiscFlags = 0;

		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		if (m_RenderTarget) texture_desc.BindFlags |= D3D11_BIND_RENDER_TARGET;

		D3D11Call(m_DeviceHandle->CreateTexture2D(&texture_desc, NULL, &m_Texture));

		D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
		ZeroMemory(&view_desc, sizeof(view_desc));
		view_desc.Format = texture_desc.Format;
		view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		view_desc.Texture2D.MostDetailedMip = 0;
		view_desc.Texture2D.MipLevels = texture_desc.MipLevels;

		D3D11Call(m_DeviceHandle->CreateShaderResourceView(m_Texture.Get(), &view_desc, &m_ResourceView));
	}

	void D3D11Texture2D::SetData(const void* data, int width, int height, bool discard)
	{
		LI_CORE_ASSERT(m_Dynamic, "Texture must be dynamic to set data.");
		LI_CORE_ASSERT(width == m_Width, "Invalid width.");
		LI_CORE_ASSERT(height == m_Height, "Invalid height.");

		D3D11_MAPPED_SUBRESOURCE resource;
		ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
		m_ContextHandle->Map(m_Texture.Get(), 0, discard ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE, 0, &resource);
		memcpy(resource.pData, data, (size_t)width * (size_t)height * (size_t)m_Channels);
		m_ContextHandle->Unmap(m_Texture.Get(), 0);
	}

	void D3D11Texture2D::Bind(uint32_t slot) const
	{
		m_ContextHandle->PSSetShaderResources(slot, 1, m_ResourceView.GetAddressOf());
		m_ContextHandle->PSSetSamplers(slot, 1, m_SamplerState.GetAddressOf());
	}

	D3D11_FILTER D3D11Texture2D::CalculateFilter(FilterType min_filter, FilterType mag_filter)
	{
		if (min_filter == FilterType::Nearest)
		{
			switch (mag_filter)
			{
			case FilterType::Nearest:
				return D3D11_FILTER_MIN_MAG_MIP_POINT;
			case FilterType::Linear:
				return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
			}
		}
		else if (min_filter == FilterType::Linear)
		{
			switch (mag_filter)
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
