#include "lipch.h"
#ifndef LI_DIST
#include "D3D11ImGuiRenderer.h"

#include "Lithium/Core/Application.h"
#include "D3D11Context.h"

#include "imgui.h"
#include "examples/imgui_impl_sdl.h"
#include "examples/imgui_impl_dx11.h"

namespace li
{
	D3D11ImGuiRenderer::D3D11ImGuiRenderer()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		ImGuiRenderer::InitImGuiStyle();

		Application* app = Application::Get();

		D3D11Context* context = (D3D11Context*)Application::Get()->GetWindow()->GetContext();

		ImGui_ImplSDL2_InitForD3D(app->GetWindow()->GetWindow());
		ImGui_ImplDX11_Init(context->GetDevice(), context->GetDeviceContext());
	}

	D3D11ImGuiRenderer::~D3D11ImGuiRenderer()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void D3D11ImGuiRenderer::Begin()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplSDL2_NewFrame(Application::Get()->GetWindow()->GetWindow());
		ImGui::NewFrame();
	}

	void D3D11ImGuiRenderer::End()
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}
#endif