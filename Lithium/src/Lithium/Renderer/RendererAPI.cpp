#include "lipch.h"
#include "RendererAPI.h"

#include "Lithium/Platform/OpenGL/OpenGLRendererAPI.h"

namespace li
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
	Scope<RendererAPI> RendererAPI::s_RendererAPI = RendererAPI::Create();

	Scope<RendererAPI> RendererAPI::Create()
	{
		//LI_CORE_INFO("CREATING RENDERER API");
		switch (s_API)
		{
		case RendererAPI::API::OpenGL:
			return CreateScope<OpenGLRendererAPI>();
		}

		LI_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}
}