#include "lipch.h"
#ifndef LI_DIST
#include "OpenGLImGuiRenderer.h"

#include "Lithium/Core/Application.h"
#include "Lithium/Platform/OpenGL/OpenGLContext.h"

#include "imgui.h"
#include "examples/imgui_impl_sdl.h"
#include "examples/imgui_impl_opengl3.h"

namespace Li
{
	OpenGLImGuiRenderer::OpenGLImGuiRenderer()
	{
		Application& app = Application::Get();

		ImGui_ImplSDL2_InitForOpenGL(app.GetWindow().GetWindow(), static_cast<OpenGLContext*>(app.GetWindow().GetContext())->GetGLContext());

		// TODO: set version dynamically for maximum compatibility.
		ImGui_ImplOpenGL3_Init("#version 420");
	}

	OpenGLImGuiRenderer::~OpenGLImGuiRenderer()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void OpenGLImGuiRenderer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(Application::Get().GetWindow().GetWindow());
		ImGui::NewFrame();
	}

	void OpenGLImGuiRenderer::End()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}
}
#endif
