#include "lipch.h"
#include "ShaderBuffer.h"

#include "Lithium/Core/Application.h"
#include "Lithium/Core/Assert.h"

#ifdef LI_INCLUDE_OPENGL
#include "Lithium/Platform/OpenGL/OpenGLShaderBuffer.h"
#endif

//#ifdef LI_INCLUDE_D3D11
//#include "Lithium/Platform/D3D11/D3D11Buffer.h"
//#endif

namespace Li
{
	Ref<ShaderBuffer> ShaderBuffer::Create(uint32_t size)
	{
		switch (Application::Get().GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::OpenGL:  return MakeRef<OpenGLShaderBuffer>(size);
#endif
//#ifdef LI_INCLUDE_D3D11
//		case RendererAPI::D3D11:  return MakeRef<D3D11ShaderBuffer>(size);
//#endif
		}

		LI_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}

