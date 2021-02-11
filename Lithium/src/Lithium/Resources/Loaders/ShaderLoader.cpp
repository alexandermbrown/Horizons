#include "lipch.h"
#include "ShaderLoader.h"

#include "Lithium/Core/Assert.h"
#include "Lithium/Core/Application.h"
#ifdef LI_INCLUDE_OPENGL
#include "Lithium/Platform/OpenGL/OpenGLShader.h"
#endif
#ifdef LI_INCLUDE_D3D11
#include "Lithium/Platform/D3D11/D3D11Shader.h"
#endif

namespace Li::Loaders
{
	static inline const char* FillGLShaderInput(const flatbuffers::String* source)
	{
		if (source)
			return source->c_str();
		return nullptr;
	}

	Ref<Shader> LoadShader(const Assets::Shader* shader)
	{
		std::string name = shader->name()->str();
		
		switch (Application::Get().GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::OpenGL:
		{
			OpenGLShaderInput input;
			input.VertexSrc = FillGLShaderInput(shader->glsl_vert());
			input.FragmentSrc = FillGLShaderInput(shader->glsl_frag());
			input.ComputeSrc = FillGLShaderInput(shader->glsl_comp());

			Ref<OpenGLShader> shader_ref = MakeRef<OpenGLShader>(name, input);
			shader_ref->Bind();
			for (const Assets::ShaderSampler* sampler : *shader->samplers())
				shader_ref->SetTexture(sampler->name()->str(), sampler->binding());

			return shader_ref;
		}
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::D3D11:
		{
			const flatbuffers::Vector<uint8_t>* hlsl_vs = shader->hlsl_vs();
			const flatbuffers::Vector<uint8_t>* hlsl_ps = shader->hlsl_ps();
			//const flatbuffers::Vector<uint8_t>* hlsl_cs = shader->hlsl_cs();
			return MakeRef<D3D11Shader>(name, hlsl_vs->data(), hlsl_vs->size(), hlsl_ps->data(), hlsl_ps->size());
		}
#endif
		}
		LI_CORE_ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}
}
