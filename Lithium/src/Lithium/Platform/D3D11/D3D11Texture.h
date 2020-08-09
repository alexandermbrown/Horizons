#pragma once

#include "Lithium/Renderer/Texture.h"

#include <d3d11.h>

namespace li
{
	class D3D11Texture2D : public Texture2D
	{
	public:
		D3D11Texture2D(int width, int height, void* data,
			WrapType wrapS, WrapType wrapT,
			FilterType minFilter, FilterType magFilter);;
		D3D11Texture2D(const std::string& path,
			WrapType wrapS, WrapType wrapT,
			FilterType minFilter, FilterType magFilter);
		D3D11Texture2D(size_t imageSize, uint8_t* rawData,
			WrapType wrapS = WrapType::ClampToEdge, WrapType wrapT = WrapType::ClampToEdge,
			FilterType minFilter = FilterType::Linear, FilterType magFilter = FilterType::Linear);

		virtual ~D3D11Texture2D();

		virtual int GetWidth() const override { return m_Width; }
		virtual int GetHeight() const override { return m_Height; }

		virtual void Resize(int width, int height) override;
		virtual void AttachToFramebuffer(
			FramebufferAttachment attachment,
			FramebufferTarget target = FramebufferTarget::Framebuffer
		) const override;

		virtual void Bind(uint32_t slot = 0) const override;

	private:
		D3D11_FILTER CalculateFilter(FilterType minFilter, FilterType magFilter);

		int m_Width;
		int m_Height;

		ID3D11SamplerState* m_SamplerState;
		ID3D11Texture2D* m_Texture;
		ID3D11ShaderResourceView* m_ResourceView;

		ID3D11Device* m_DeviceHandle;
		ID3D11DeviceContext* m_ContextHandle;
	};
}
