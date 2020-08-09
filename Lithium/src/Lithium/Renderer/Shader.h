#pragma once

#include "Lithium/Core/Core.h"
#include "UniformBuffer.h"

#include <string>

#include <glm/glm.hpp>

namespace li 
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;

		virtual void AddUniformBuffer(const Ref<UniformBuffer>& buffer) = 0;
		
		virtual void SetTexture(const std::string& name, int slot) = 0;

		virtual const std::string& GetName() const = 0;
	};
}