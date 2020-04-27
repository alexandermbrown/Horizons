#include "lipch.h"
#include "VertexArray.h"

#include "Lithium/Renderer/Renderer.h"
#include "Lithium/Renderer/RendererAPI.h"
#include "Lithium/Platform/OpenGL/OpenGLVertexArray.h"

namespace li
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexArray>();
		}

		LI_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}
}