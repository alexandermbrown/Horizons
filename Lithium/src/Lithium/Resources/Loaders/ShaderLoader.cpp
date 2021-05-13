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
	static inline const char* FillGLSLInput(const flatbuffers::String* source)
	{
		if (source)
			return source->c_str();
		return nullptr;
	}

	static inline void FillDXILInput(const uint8_t** data, uint32_t* size, const flatbuffers::Vector<uint8_t>* dxil)
	{
		if (dxil)
		{
			*data = dxil->data();
			*size = dxil->size();
		}
	}

	Ref<Shader> LoadShader(const Assets::Shader* shader)
	{
		std::string name = shader->name()->str();
		
		switch (Application::Get().GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::OpenGL:
		{
			GLSLInput input;
			input.VertexSrc = FillGLSLInput(shader->glsl_vert());
			input.FragmentSrc = FillGLSLInput(shader->glsl_frag());
			input.ComputeSrc = FillGLSLInput(shader->glsl_comp());

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
			DXILInput input;
			FillDXILInput(&input.VSData, &input.VSSize, shader->hlsl_vs());
			FillDXILInput(&input.PSData, &input.PSSize, shader->hlsl_ps());
			FillDXILInput(&input.CSData, &input.CSSize, shader->hlsl_cs());

			return MakeRef<D3D11Shader>(name, input);
		}
#endif
		}
		LI_CORE_ASSERT(false, "Unknown renderer API.");
		return nullptr;
	}
}
