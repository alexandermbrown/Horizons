#pragma once

#include "Lithium/Core/Core.h"
#include "UniformBuffer.h"

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace li 
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddUniformBuffer(const std::shared_ptr<UniformBuffer>& buffer) = 0;
		
		virtual void SetTexture(const std::string& name, int slot) = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		static Ref<Shader> Create(const std::string& name, const std::string& source);
	};
}