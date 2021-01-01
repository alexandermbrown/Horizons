#include "lipch.h"
#include "Framebuffer.h"

#include "Lithium/Core/Application.h"

#ifdef LI_INCLUDE_OPENGL
#include "Lithium/Platform/OpenGL/OpenGLFramebuffer.h"
#endif
#ifdef LI_INCLUDE_D3D11
#include "Lithium/Platform/D3D11/D3D11Framebuffer.h"
#endif

namespace Li
{
	Ref<Framebuffer> Framebuffer::Create(int width, int height)
	{
		switch (Application::Get().GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::OpenGL:
			return MakeRef<OpenGLFramebuffer>(width, height);
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::D3D11:
			return MakeRef<D3D11Framebuffer>(width, height);
#endif
		}

		LI_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}
}