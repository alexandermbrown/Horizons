#include "lipch.h"
#ifndef LI_DIST
#include "D3D11ImGuiRenderer.h"

#include "Lithium/Core/Application.h"
#include "D3D11Context.h"

#include "imgui.h"
#include "examples/imgui_impl_sdl.h"
#include "examples/imgui_impl_dx11.h"

namespace Li
{
	D3D11ImGuiRenderer::D3D11ImGuiRenderer()
	{
		Application& app = Application::Get();
		D3D11Context* context = (D3D11Context*)Application::Get().GetWindow().GetContext();

		ImGui_ImplSDL2_InitForD3D(app.GetWindow().GetWindow());
		ImGui_ImplDX11_Init(context->GetDevice().Get(), context->GetDeviceContext().Get());
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
		ImGui_ImplSDL2_NewFrame(Application::Get().GetWindow().GetWindow());
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
