#pragma once

#include "Lithium/Renderer/Shader.h"
#include <glm/glm.hpp>

#include <d3d11.h>
#include <filesystem>

namespace li
{
	class D3D11Shader : public Shader
	{
	public:
#ifdef LI_DEBUG
		D3D11Shader(const std::string& name, const std::filesystem::path& filepath);
#endif

		D3D11Shader(const std::string& name, void* vs_bytecode, uint32_t vs_size, void* ps_bytecode, uint32_t ps_size);
		virtual ~D3D11Shader();

		virtual void Bind() const override;

		virtual void AddUniformBuffer(const Ref<UniformBuffer>& buffer) override;

		virtual void SetTexture(const std::string& name, int slot) override;

		virtual const std::string& GetName() const override { return m_Name; }

		void* GetVSBufferData() const { return m_VSBufferData; }
		uint32_t GetVSBufferSize() const { return m_VSBufferSize; }

	private:

		std::string m_Name;

		ID3D11VertexShader* m_VertexShader = nullptr;
		ID3D11PixelShader* m_PixelShader = nullptr;

		void* m_VSBufferData = nullptr;
		uint32_t m_VSBufferSize = 0;

		std::vector<ID3D11Buffer*> m_VSUniformBuffers;
		std::vector<ID3D11Buffer*> m_PSUniformBuffers;

		ID3D11Device* m_DeviceHandle;
		ID3D11DeviceContext* m_ContextHandle;
	};
}