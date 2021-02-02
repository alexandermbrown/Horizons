#pragma once

#include "Lithium/Core/Memory.h"
#include "UniformBuffer.h"

#include "glm/glm.hpp"
#include <string>

namespace Li 
{
	struct OpenGLShaderInput
	{
		const char* VertexSrc{ nullptr };
		const char* TessControlSrc{ nullptr };
		const char* TessEvalSrc{ nullptr };
		const char* GeometrySrc{ nullptr };
		const char* FragmentSrc{ nullptr };
		const char* ComputeSrc{ nullptr };
	};

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void SetTexture(const std::string& name, int slot) = 0;
		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& name, const OpenGLShaderInput& gl_input, const uint8_t* vs_bytecode, uint32_t vs_size, const uint8_t* ps_bytecode, uint32_t ps_size);
	};
}
