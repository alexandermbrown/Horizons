#include "lipch.h"
#include "Framebuffer.h"

#include "Lithium/Renderer/Renderer.h"
#include "Lithium/Renderer/RendererAPI.h"

#ifdef LI_INCLUDE_OPENGL
#include "Lithium/Platform/OpenGL/OpenGLFramebuffer.h"
#endif
#ifdef LI_INCLUDE_D3D11
#include "Lithium/Platform/D3D11/D3D11Framebuffer.h"
#endif

namespace li
{
	Ref<Framebuffer> Framebuffer::Create(int width, int height)
	{
		switch (Renderer::GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFramebuffer>(width, height);
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::API::D3D11:
			return CreateRef<D3D11Framebuffer>(width, height);
#endif
		}

		LI_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}
}