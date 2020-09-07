#pragma once

#include "Lithium/Renderer/Framebuffer.h"
#include "Lithium/Platform/D3D11/D3D11Texture.h"

#include <d3d11.h>

namespace li
{
	class D3D11Framebuffer : public Framebuffer
	{
	public:
		D3D11Framebuffer(int width, int height);
		virtual ~D3D11Framebuffer();

		virtual void Bind() const override;
		virtual void SetClearColor(const glm::vec4& color) override { m_ClearColor = color; }
		virtual void Clear() const override;

		inline virtual Ref<Texture2D> GetTexture() const override { return m_Texture; };
		virtual void Resize(int width, int height) override;
		virtual glm::ivec2 GetSize() const override { return m_Size; };

	private:
		void CreateBuffers(int width, int height);

		Ref<D3D11Texture2D> m_Texture;

		ID3D11RenderTargetView* m_RenderTargetView;
		ID3D11Device* m_DeviceHandle;
		ID3D11DeviceContext* m_ContextHandle;
		ID3D11Texture2D* m_DepthStencilBuffer;
		ID3D11DepthStencilView* m_DepthStencilView;

		D3D11_VIEWPORT m_Viewport;

		glm::vec4 m_ClearColor;
		glm::ivec2 m_Size;
	};
}