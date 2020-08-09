#include "lipch.h"
#include "VertexArray.h"

#include "Lithium/Renderer/Renderer.h"
#include "Lithium/Renderer/RendererAPI.h"

#ifdef LI_INCLUDE_OPENGL
#include "Lithium/Platform/OpenGL/OpenGLVertexArray.h"
#endif

#ifdef LI_INCLUDE_D3D11
#include "Lithium/Platform/D3D11/D3D11VertexArray.h"
#endif

namespace li
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexArray>();
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::API::D3D11:
			return CreateRef<D3D11VertexArray>();
#endif
		}

		LI_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}
}