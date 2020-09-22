#include "pch.h"
#ifndef LI_DIST
#include "LevelEditorLayer.h"

#include "imgui.h"
#include "nfd.h"

#ifdef LI_PLATFORM_WINDOWS
#include <Windows.h>
#include <tchar.h>
#endif

LevelEditorLayer::LevelEditorLayer()
	: m_ReturnToMainMenu(false), m_DockspaceOpen(true), m_Viewport(&m_Settings), m_TerrainEditingPanel(&m_Settings)
{
	m_Settings.Brush.Enabled = true;
	m_Settings.Brush.Subtract = false;
	m_Settings.Brush.Strength = 0.25f;
	m_Settings.Brush.InnerRadius = 1.0f;
	m_Settings.Brush.OuterRadius = 4.0f;
	m_Settings.Display.ShowChunkBorders = true;
	m_Settings.Display.VertexDisplayMode = VertexDisplayMode::ShowInBrush;
}

LevelEditorLayer::~LevelEditorLayer()
{
}

void LevelEditorLayer::OnAttach()
{
	SDL_SetWindowTitle(li::Application::Get()->GetWindow()->GetWindow(), "Horizons Level Editor");
}

void LevelEditorLayer::OnDetach()
{
	SDL_SetWindowTitle(li::Application::Get()->GetWindow()->GetWindow(), "Horizons");
}

void LevelEditorLayer::OnUpdate(float dt)
{
	m_Viewport.OnUpdate(dt);

	li::Application::Get()->GetWindow()->GetContext()->BindDefaultRenderTarget();
	li::Application::Get()->GetWindow()->GetContext()->Clear();
}

void LevelEditorLayer::OnImGuiRender()
{
	constexpr ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
		| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->GetWorkPos());
	ImGui::SetNextWindowSize(viewport->GetWorkSize());
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &m_DockspaceOpen, window_flags);
	ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(u8"File"))
		{
			if (ImGui::MenuItem("New", "Ctrl+N")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O"))
			{
				nfdchar_t* path;
				nfdresult_t result = NFD_OpenDialog("terrain", NULL, &path);
				if (result == NFD_OKAY)
				{
					m_Viewport.FileOpen(path);
				}
				else if (result == NFD_ERROR)
				{
					LI_ERROR(NFD_GetError());
				}
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Save", "Ctrl+S")) {}
			if (ImGui::MenuItem("Save As", "Ctrl+Shift+S")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Return to Main Menu")) m_ReturnToMainMenu = true;
			if (ImGui::MenuItem("Exit")) li::Application::Get()->Exit(); 

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
			if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
			if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Viewport")) m_Viewport.ShowPanel();
			if (ImGui::MenuItem("Terrain Editing")) m_TerrainEditingPanel.ShowPanel();
			if (ImGui::MenuItem("Open Scripts in VS Code"))
			{
#ifdef LI_PLATFORM_WINDOWS
				STARTUPINFO si;
				ZeroMemory(&si, sizeof(si));
				si.cb = sizeof(si);

				PROCESS_INFORMATION pi;
				ZeroMemory(&pi, sizeof(pi));
				LPWSTR cmd = _tcsdup(TEXT("code.cmd .\\data\\scripts"));
				if (CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
				{
					LI_RUN_ASSERT(CloseHandle(pi.hProcess), "Failed to close process handle.");
					LI_RUN_ASSERT(CloseHandle(pi.hThread), "Failed to close thread handle.");
					LI_INFO("Successfully launched Visual Studio Code.");
				}
				else LI_ERROR("Failed to launch vs code: error {}", GetLastError());
#endif
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	ImGui::End();

	m_TerrainEditingPanel.OnImGuiRender();
	m_Viewport.OnImGuiRender();
}

void LevelEditorLayer::OnEvent(SDL_Event* event)
{
	m_Viewport.OnEvent(event);
}
#endif