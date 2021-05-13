#pragma once

#include "Lithium/Core/Memory.h"
#include "Lithium/Renderer/RendererEnums.h"

#include "glm/glm.hpp"
#include <string>
#include <vector>
#include <unordered_map>

namespace Li
{
	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;

		virtual void Bind(ShaderType type) const = 0;
		virtual void BindToSlot() const = 0;
		virtual uint32_t GetSlot() const = 0;
		virtual void SetData(const void* data) = 0;

		static Ref<UniformBuffer> Create(uint32_t slot, uint32_t size);
	};
}
