#include "lipch.h"
#include "Framebuffer.h"

#include "Lithium/Renderer/Renderer.h"
#include "Lithium/Renderer/RendererAPI.h"
#include "Lithium/Platform/OpenGL/OpenGLFramebuffer.h"

namespace li
{
	Ref<Framebuffer> Framebuffer::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFramebuffer>();
		}

		LI_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}
}