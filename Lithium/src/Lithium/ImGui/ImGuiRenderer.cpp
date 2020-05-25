#include "lipch.h"
#include "Lithium/ImGui/ImGuiRenderer.h"

#include <imgui.h>
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_sdl.h>

#include "Lithium/Core/Application.h"

// TEMPORARY
#include <glad/glad.h>
#include "Lithium/Platform/OpenGL/OpenGLContext.h"

namespace li 
{
	ImGuiRenderer::ImGuiRenderer()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		Application& app = Application::Get();

		// Setup Platform/Renderer bindings
		ImGui_ImplSDL2_InitForOpenGL((SDL_Window*)app.GetWindow()->GetWindow(), std::dynamic_pointer_cast<OpenGLContext>(app.GetWindow()->GetContext())->GetGLContext());
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	ImGuiRenderer::~ImGuiRenderer()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}
	
	void ImGuiRenderer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame((SDL_Window*)Application::Get().GetWindow()->GetWindow());
		ImGui::NewFrame();
	}

	void ImGuiRenderer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow()->GetWidth(), (float)app.GetWindow()->GetHeight());

		// Rendering
		ImGui::EndScene();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiRenderer::OnEvent(SDL_Event* event)
	{
		ImGui_ImplSDL2_ProcessEvent(event);
	}

	bool ImGuiRenderer::WantCapture(const SDL_Event& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureMouse) {
			if (event.type == SDL_MOUSEBUTTONDOWN ||
				event.type == SDL_MOUSEBUTTONUP ||
				event.type == SDL_MOUSEMOTION ||
				event.type == SDL_MOUSEWHEEL)
				return true;
		}

		if (io.WantCaptureKeyboard) {
			if (event.type == SDL_KEYDOWN ||
				event.type == SDL_KEYUP ||
				event.type == SDL_TEXTEDITING ||
				event.type == SDL_TEXTINPUT)
				return true;
		}
		return false;
	}
}