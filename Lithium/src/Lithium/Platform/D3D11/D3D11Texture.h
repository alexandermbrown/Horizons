#pragma once

#include "Lithium/Renderer/Texture.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace Li
{
	class D3D11Texture2D : public Texture2D
	{
	public:
		D3D11Texture2D(int width, int height, int channels, void* data, WrapType wrap_s, WrapType wrap_t,
			FilterType min_filter, FilterType magFilter, bool dynamic, bool render_target);

		D3D11Texture2D(const std::string& path, int desired_channels, WrapType wrap_s, WrapType wrap_t,
			FilterType min_filter, FilterType mag_filter);

		D3D11Texture2D(size_t image_size, const uint8_t* rawData, int desired_channels, WrapType wrap_s, WrapType wrap_t,
			FilterType min_filter, FilterType mag_filter);

		virtual ~D3D11Texture2D() = default;

		virtual int GetWidth() const override { return m_Width; }
		virtual int GetHeight() const override { return m_Height; }

		virtual void Bind(uint32_t slot = 0) const override;
		virtual void Resize(int width, int height) override;
		virtual void SetData(const void* data, int width, int height, bool discard) override;
		virtual void AttachToFramebuffer() const override {};
		virtual void* GetInternalTexture() const override { return static_cast<void*>(m_ResourceView.Get()); }
		ID3D11Texture2D* GetTexture() { return m_Texture.Get(); }

	private:
		D3D11_FILTER CalculateFilter(FilterType minFilter, FilterType magFilter);

		int m_Width;
		int m_Height;
		int m_Channels;

		bool m_Dynamic;
		bool m_RenderTarget;

		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SamplerState;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_Texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ResourceView;

		Microsoft::WRL::ComPtr<ID3D11Device> m_DeviceHandle;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_ContextHandle;
	};
}
