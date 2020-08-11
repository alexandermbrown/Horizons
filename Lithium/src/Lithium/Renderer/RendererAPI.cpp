#include "lipch.h"
#include "RendererAPI.h"

#ifdef LI_INCLUDE_OPENGL
#include "Lithium/Platform/OpenGL/OpenGLRendererAPI.h"
#endif

#ifdef LI_INCLUDE_D3D11
#include "Lithium/Platform/D3D11/D3D11RendererAPI.h"
#endif

namespace li
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::None;
	Scope<RendererAPI> RendererAPI::s_RendererAPI = nullptr;

	void RendererAPI::Create(RendererAPI::API api)
	{
		LI_CORE_ASSERT(s_API == RendererAPI::API::None, "Already initialized!");

		s_API = api;
		switch (s_API)
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::API::OpenGL:
			s_RendererAPI = CreateScope<OpenGLRendererAPI>();
			break;
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::API::D3D11:
			s_RendererAPI = CreateScope<D3D11RendererAPI>();
			 break;
#endif
		default:
			LI_CORE_ASSERT(false, "Unknown RendererAPI!");
		}
	}
}