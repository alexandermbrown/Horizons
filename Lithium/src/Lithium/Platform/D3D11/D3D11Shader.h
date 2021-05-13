#pragma once

#include "Lithium/Renderer/Shader.h"
#include <glm/glm.hpp>

#include <d3d11.h>
#include <wrl/client.h>
#include <filesystem>

namespace Li
{
	struct DXILInput
	{
		const uint8_t* VSData{ nullptr };
		const uint8_t* PSData{ nullptr };
		const uint8_t* CSData{ nullptr };
		uint32_t VSSize{ 0 };
		uint32_t PSSize{ 0 };
		uint32_t CSSize{ 0 };
	};

	class D3D11Shader : public Shader
	{
	public:
		D3D11Shader(const std::string& name, const DXILInput& input);
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
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_ComputeShader;

		std::vector<uint8_t> m_VSBufferData;

		Microsoft::WRL::ComPtr<ID3D11Device> m_DeviceHandle;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_ContextHandle;
	};
}
