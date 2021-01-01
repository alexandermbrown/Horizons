#pragma once

#include "Lithium/Core/Core.h"
#include "UniformBuffer.h"

#include "glm/glm.hpp"
#include <string>

namespace Li 
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		
		virtual void SetTexture(const std::string& name, int slot) = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& name, const std::string& glsl, const void* vs_bytecode, uint32_t vs_size, const void* ps_bytecode, uint32_t ps_size);
	};
}