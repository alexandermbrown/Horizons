#include "lipch.h"
#include "Shader.h"

#include "Lithium/Core/Application.h"
#ifdef LI_INCLUDE_OPENGL
#include "Lithium/Platform/OpenGL/OpenGLShader.h"
#endif
#ifdef LI_INCLUDE_D3D11
#include "Lithium/Platform/D3D11/D3D11Shader.h"
#endif

namespace Li
{
	Ref<Shader> Shader::Create(const std::string& name, const std::string& glsl, const uint8_t* vs_bytecode, uint32_t vs_size, const uint8_t* ps_bytecode, uint32_t ps_size)
	{
		switch (Application::Get().GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::OpenGL:
			return MakeRef<OpenGLShader>(name, glsl);
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::D3D11:
			return MakeRef<D3D11Shader>(name, vs_bytecode, vs_size, ps_bytecode, ps_size);
#endif
		}

		Log::CoreError("Unknown RendererAPI!");
		return nullptr;
	}
}
