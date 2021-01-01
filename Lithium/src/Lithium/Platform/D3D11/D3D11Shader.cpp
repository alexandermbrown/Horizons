#include "lipch.h"
#include "D3D11Shader.h"

#include "D3D11Core.h"
#include "D3D11Context.h"
#include "D3D11UniformBuffer.h"
#include "Lithium/Core/Application.h"

#ifdef LI_DEBUG
#include <d3dcompiler.h>
#endif

#include <algorithm>

namespace Li
{
#ifdef LI_DEBUG
	D3D11Shader::D3D11Shader(const std::string& name, const std::filesystem::path& filepath)
		: m_Name(name)
	{
		D3D11Context* context = (D3D11Context*)Application::Get().GetWindow().GetContext();
		m_DeviceHandle = context->GetDevice();
		m_ContextHandle = context->GetDeviceContext();

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef LI_DEBUG
		flags |= D3DCOMPILE_DEBUG;
#else
		flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

		ID3D10Blob* vertexShaderBuffer = nullptr;
		ID3D10Blob* pixelShaderBuffer = nullptr;

		ID3D10Blob* errorMessage = nullptr;

		HRESULT result = D3DCompileFromFile(filepath.c_str(), NULL, NULL, "vs_main", "vs_5_0", flags, 0, &vertexShaderBuffer, &errorMessage);
		if (FAILED(result))
		{
			std::cout.write((const char*)errorMessage->GetBufferPointer(), errorMessage->GetBufferSize());
			return;
		}

		result = D3DCompileFromFile(filepath.c_str(), NULL, NULL, "ps_main", "ps_5_0", flags, 0, &pixelShaderBuffer, &errorMessage);

		if (FAILED(result))
		{
			std::cout.write((const char*)errorMessage->GetBufferPointer(), errorMessage->GetBufferSize());
			return;
		}

		D3D11Call( m_DeviceHandle->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_VertexShader) );
		D3D11Call( m_DeviceHandle->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_PixelShader) );

		vertexShaderBuffer->Release();
		pixelShaderBuffer->Release();
	}
#endif

	D3D11Shader::D3D11Shader(const std::string& name, const void* vs_bytecode, uint32_t vs_size, const void* ps_bytecode, uint32_t ps_size)
		: m_Name(name)
	{
		m_VSBufferSize = vs_size;
		m_VSBufferData = new uint8_t[vs_size];
		memcpy(m_VSBufferData, vs_bytecode, vs_size);

		D3D11Context* context = (D3D11Context*)Application::Get().GetWindow().GetContext();
		m_DeviceHandle = context->GetDevice();
		m_ContextHandle = context->GetDeviceContext();

		D3D11Call( m_DeviceHandle->CreateVertexShader(vs_bytecode, vs_size, NULL, &m_VertexShader) );
		D3D11Call( m_DeviceHandle->CreatePixelShader(ps_bytecode, ps_size, NULL, &m_PixelShader) );
	}

	D3D11Shader::~D3D11Shader()
	{
		delete[] m_VSBufferData;
		LI_D3D_RELEASE(m_VertexShader);
		LI_D3D_RELEASE(m_PixelShader);
	}

	void D3D11Shader::Bind() const
	{
		m_ContextHandle->VSSetShader(m_VertexShader, NULL, 0);
		m_ContextHandle->PSSetShader(m_PixelShader, NULL, 0);
	}
}