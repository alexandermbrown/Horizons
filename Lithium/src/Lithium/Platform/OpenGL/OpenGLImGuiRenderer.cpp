#include "lipch.h"
#include "OpenGLImGuiRenderer.h"

#include "Lithium/Core/Application.h"
#include "Lithium/Platform/OpenGL/OpenGLContext.h"

#include "imgui.h"
#include "examples/imgui_impl_sdl.h"
#include "examples/imgui_impl_opengl3.h"

namespace li
{
	OpenGLImGuiRenderer::OpenGLImGuiRenderer()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		ImGui::StyleColorsDark();

		Application* app = Application::Get();

		ImGui_ImplSDL2_InitForOpenGL(app->GetWindow()->GetWindow(), ((OpenGLContext*)app->GetWindow()->GetContext())->GetGLContext());

		// TODO: set version dynamically for maximum compatibility.
		ImGui_ImplOpenGL3_Init("#version 410");
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
		ImGui_ImplSDL2_NewFrame(Application::Get()->GetWindow()->GetWindow());
		ImGui::NewFrame();
	}

	void OpenGLImGuiRenderer::End()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}