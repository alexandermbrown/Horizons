#include "lipch.h"
#include "Shader.h"

#include "RendererAPI.h"
#ifdef LI_INCLUDE_OPENGL
#include "Lithium/Platform/OpenGL/OpenGLShader.h"
#endif
#ifdef LI_INCLUDE_D3D11
#include "Lithium/Platform/D3D11/D3D11Shader.h"
#endif

namespace li
{
	Ref<Shader> Shader::Create(const std::string& name, const std::string& glsl, const void* vs_bytecode, uint32_t vs_size, const void* ps_bytecode, uint32_t ps_size)
	{
		switch (RendererAPI::GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLShader>(name, glsl);
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::API::D3D11:
			return CreateRef<D3D11Shader>(name, vs_bytecode, vs_size, ps_bytecode, ps_size);
#endif
		}

		LI_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}
}
