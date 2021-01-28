#pragma once

#include "Lithium/Renderer/Shader.h"
#include <glm/glm.hpp>

#include <d3d11.h>
#include <wrl/client.h>
#include <filesystem>

namespace Li
{
	class D3D11Shader : public Shader
	{
	public:
		D3D11Shader(const std::string& name, const uint8_t* vs_bytecode, uint32_t vs_size, const uint8_t* ps_bytecode, uint32_t ps_size);
		virtual ~D3D11Shader() = default;

		virtual void Bind() const override;

		virtual void SetTexture(const std::string& name, int slot) override {};

		virtual const std::string& GetName() const override { return m_Name; }

		const void* GetVSBufferData() const { return m_VSBufferData.data(); }
		size_t GetVSBufferSize() const { return m_VSBufferData.size(); }

	private:
		std::string m_Name;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;

		std::vector<uint8_t> m_VSBufferData;

		Microsoft::WRL::ComPtr<ID3D11Device> m_DeviceHandle;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_ContextHandle;
	};
}
