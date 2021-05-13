#pragma once

#include "Lithium/Renderer/UniformBuffer.h"
#include "Lithium/Core/Assert.h"

#include <d3d11.h>
#include <wrl/client.h>

#include <unordered_map>
#include <string>

namespace Li
{
	class D3D11UniformBuffer : public UniformBuffer
	{
	public:
		D3D11UniformBuffer(uint32_t slot, uint32_t size);
		virtual ~D3D11UniformBuffer() = default;

		virtual void Bind(ShaderType type) const override;
		virtual void BindToSlot() const override {};
		virtual inline uint32_t GetSlot() const override { return m_Slot; }
		virtual void SetData(const void* data) override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer;
		uint32_t m_Slot;
		uint32_t m_Size;

		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_ContextHandle;
	};
}
