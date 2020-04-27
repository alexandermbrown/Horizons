#include "lipch.h"
#include "Renderbuffer.h"

#include "Lithium/Renderer/Renderer.h"
#include "Lithium/Renderer/RendererAPI.h"
#include "Lithium/Platform/OpenGL/OpenGLRenderbuffer.h"

namespace li
{
	Ref<Renderbuffer> Renderbuffer::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLRenderbuffer>();
		}

		LI_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}
}