#include "lipch.h"
#include "VertexArray.h"

#include "Lithium/Core/Application.h"

#ifdef LI_INCLUDE_OPENGL
#include "Lithium/Platform/OpenGL/OpenGLVertexArray.h"
#endif

#ifdef LI_INCLUDE_D3D11
#include "Lithium/Platform/D3D11/D3D11VertexArray.h"
#endif

namespace Li
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Application::Get().GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::OpenGL:
			return MakeRef<OpenGLVertexArray>();
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::D3D11:
			return MakeRef<D3D11VertexArray>();
#endif
		}

		Log::CoreError("Unknown RendererAPI!");
		return nullptr;
	}
}