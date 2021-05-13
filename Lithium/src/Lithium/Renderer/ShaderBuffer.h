#pragma once

#include "Lithium/Core/Memory.h"
#include <stdint.h>

namespace Li
{
	class ShaderBuffer
	{
	public:
		virtual ~ShaderBuffer() = default;

		virtual void Bind(uint32_t slot) const = 0;
		virtual void SetSubData(uint32_t* data, uint32_t size, uint32_t offset, bool discard) = 0;

		static Ref<ShaderBuffer> Create(uint32_t size);
	};
}
