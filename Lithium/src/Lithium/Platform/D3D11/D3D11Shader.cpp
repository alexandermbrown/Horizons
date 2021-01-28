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

using namespace Microsoft::WRL;

namespace Li
{
	D3D11Shader::D3D11Shader(const std::string& name, const uint8_t* vs_bytecode, uint32_t vs_size, const uint8_t* ps_bytecode, uint32_t ps_size)
		: m_Name(name), m_VSBufferData(vs_bytecode, vs_bytecode + vs_size)
	{
		D3D11Context* context = (D3D11Context*)Application::Get().GetWindow().GetContext();
		m_DeviceHandle = context->GetDevice();
		m_ContextHandle = context->GetDeviceContext();

		D3D11Call( m_DeviceHandle->CreateVertexShader(vs_bytecode, vs_size, NULL, &m_VertexShader) );
		D3D11Call( m_DeviceHandle->CreatePixelShader(ps_bytecode, ps_size, NULL, &m_PixelShader) );
	}

	void D3D11Shader::Bind() const
	{
		m_ContextHandle->VSSetShader(m_VertexShader.Get(), NULL, 0);
		m_ContextHandle->PSSetShader(m_PixelShader.Get(), NULL, 0);
	}
}
