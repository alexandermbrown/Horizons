#include "lipch.h"
#include "Window.h"

#include "Lithium/Platform/OpenGL/OpenGLWindow.h"

namespace li 
{
	Ref<Window> Window::Create(const char* title, int width, int height, bool resizable, bool shown, RendererAPI::API api)
	{
		switch (api)
		{
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLWindow>(title, width, height, resizable, shown);
			break;
		default:
			LI_CORE_ERROR("Unknown graphics API");
			return nullptr;
		}
	}
}

