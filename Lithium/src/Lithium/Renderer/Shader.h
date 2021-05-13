#pragma once

#include "Lithium/Core/Memory.h"
#include "UniformBuffer.h"

#include "glm/glm.hpp"
#include <string>

namespace Li 
{
	/*struct GLSLInput
	{
		const char* VertexSrc{ nullptr };
		const char* TessControlSrc{ nullptr };
		const char* TessEvalSrc{ nullptr };
		const char* GeometrySrc{ nullptr };
		const char* FragmentSrc{ nullptr };
		const char* ComputeSrc{ nullptr };
	};

	struct DXILInput
	{
		const uint8_t* VSData{ nullptr };
		const uint8_t* PSData{ nullptr };
		const uint8_t* CSData{ nullptr };
		uint32_t VSSize{ 0 };
		uint32_t PSSize{ 0 };
		uint32_t CSSize{ 0 };
	};*/

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual const std::string& GetName() const = 0;
		virtual void SetTexture(const std::string& name, int slot) = 0;

		//static Ref<Shader> Create(const std::string& name, const GLSLInput& glsl_input, const DXILInput& dxil_input);
	};
}
