#include "lipch.h"
#include "D3D11Shader.h"

#include "D3D11Core.h"
#include "D3D11Context.h"
#include "D3D11UniformBuffer.h"
#include "Lithium/Core/Application.h"

#include <algorithm>

using namespace Microsoft::WRL;

namespace Li
{
	D3D11Shader::D3D11Shader(const std::string& name, const DXILInput& input)
		: m_Name(name)
	{
		D3D11Context* context = static_cast<D3D11Context*>(Application::Get().GetWindow().GetContext());
		m_DeviceHandle = context->GetDevice();
		m_ContextHandle = context->GetDeviceContext();

		if (input.VSData)
		{
			m_VSBufferData = std::vector(input.VSData, input.VSData + input.VSSize);
			D3D11Call( m_DeviceHandle->CreateVertexShader(input.VSData, input.VSSize, NULL, &m_VertexShader) );
		}
		if (input.PSData)
		{
			D3D11Call( m_DeviceHandle->CreatePixelShader(input.PSData, input.PSSize, NULL, &m_PixelShader) );
		}
		if (input.CSData)
		{
			D3D11Call( m_DeviceHandle->CreateComputeShader(input.CSData, input.CSSize, NULL, &m_ComputeShader) );
		}
	}

	void D3D11Shader::Bind() const
	{
		if (m_VertexShader)
			m_ContextHandle->VSSetShader(m_VertexShader.Get(), NULL, 0);
		if (m_PixelShader)
			m_ContextHandle->PSSetShader(m_PixelShader.Get(), NULL, 0);
		if (m_ComputeShader)
			m_ContextHandle->CSSetShader(m_ComputeShader.Get(), NULL, 0);
	}
}
