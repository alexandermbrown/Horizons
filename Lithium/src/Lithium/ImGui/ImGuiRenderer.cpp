#include "lipch.h"
#ifndef LI_DIST
#include "Lithium/ImGui/ImGuiRenderer.h"

#include "imgui.h"
#include "examples/imgui_impl_sdl.h"
#include "Lithium/Core/Application.h"

#ifdef LI_INCLUDE_OPENGL
#include "Lithium/Platform/OpenGL/OpenGLImGuiRenderer.h"
#endif

#ifdef LI_INCLUDE_D3D11
#include "Lithium/Platform/D3D11/D3D11ImGuiRenderer.h"
#endif

namespace li 
{
	void ImGuiRenderer::OnEvent(SDL_Event* event)
	{
		ImGui_ImplSDL2_ProcessEvent(event);
		ImGuiIO& io = ImGui::GetIO();

		if (io.WantCaptureKeyboard) {
			if (event->type == SDL_KEYDOWN ||
				event->type == SDL_KEYUP ||
				event->type == SDL_TEXTEDITING ||
				event->type == SDL_TEXTINPUT)
			{
				Application::Get()->EventHandled();
			}
		}
		
		if (io.WantCaptureMouse) {
			if (event->type == SDL_MOUSEBUTTONDOWN ||
				event->type == SDL_MOUSEBUTTONUP ||
				event->type == SDL_MOUSEMOTION ||
				event->type == SDL_MOUSEWHEEL)
			{
				Application::Get()->EventHandled();
			}
		}
	}

	void ImGuiRenderer::Resize(int width, int height)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)width, (float)height);
	}

	Ref<ImGuiRenderer> ImGuiRenderer::Create()
	{
		switch (Application::Get()->GetAPI())
		{
#ifdef LI_INCLUDE_OPENGL
		case RendererAPI::OpenGL:  return CreateRef<OpenGLImGuiRenderer>();
#endif
#ifdef LI_INCLUDE_D3D11
		case RendererAPI::D3D11:  return CreateRef<D3D11ImGuiRenderer>();
#endif
		}

		LI_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
#endif