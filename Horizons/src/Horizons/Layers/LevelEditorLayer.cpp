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
	: m_ReturnToMainMenu(false), m_DockspaceOpen(true), m_TitleHasAsterisk(false), m_Viewport(&m_Settings), m_TerrainEditingPanel(&m_Settings)
{
	m_Settings.Brush.Enabled = true;
	m_Settings.Brush.Subtract = false;
	m_Settings.Brush.Strength = 1.0f;
	m_Settings.Brush.InnerRadius = 1.0f;
	m_Settings.Brush.OuterRadius = 4.0f;
	m_Settings.Display.ShowChunkBorders = true;
	m_Settings.Display.VertexDisplayMode = VertexDisplayMode::ShowInBrush;
	m_Settings.Layer = 0;
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
	if (!m_TitleHasAsterisk && m_Viewport.IsTerrainModified())
	{
		SDL_SetWindowTitle(li::Application::Get()->GetWindow()->GetWindow(), (m_TerrainPath.filename().string() + "* - Horizons Level Editor").c_str());
		m_TitleHasAsterisk = true;
	}
	else if (m_TitleHasAsterisk && !m_Viewport.IsTerrainModified())
	{
		SDL_SetWindowTitle(li::Application::Get()->GetWindow()->GetWindow(), (m_TerrainPath.filename().string() + " - Horizons Level Editor").c_str());
		m_TitleHasAsterisk = false;
	}

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
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", "Ctrl+N")) FileNew();
			if (ImGui::MenuItem("Open", "Ctrl+O")) FileOpen();
			ImGui::Separator();
			if (ImGui::MenuItem("Save", "Ctrl+S")) FileSave();
			if (ImGui::MenuItem("Save As", "Ctrl+Shift+S")) FileSaveAs();
			ImGui::Separator();
			if (ImGui::MenuItem("Return to Main Menu"))
			{
				if (m_Viewport.IsTerrainOpen() && m_Viewport.IsTerrainModified())
				{
					int button_id;
					UnsavedChangesDialog(&button_id);
					// If save is clicked.
					if (button_id == 0)
					{
						FileSave();
						m_ReturnToMainMenu = true;
					}
					// If cancel is clicked.
					else if (button_id == 1)
						m_ReturnToMainMenu = true;
				}
				else m_ReturnToMainMenu = true;
			}
			if (ImGui::MenuItem("Exit", "Alt+F4"))
			{
				if (m_Viewport.IsTerrainOpen() && m_Viewport.IsTerrainModified())
				{
					int button_id;
					UnsavedChangesDialog(&button_id);
					// If save is clicked.
					if (button_id == 0)
					{
						FileSave();
						li::Application::Get()->Exit();
					}
					// If cancel is clicked.
					else if (button_id == 1)
						li::Application::Get()->Exit();
				}
				else li::Application::Get()->Exit();
			}

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

	if (m_Viewport.IsTerrainOpen())
	{
		glm::ivec2 world_size = m_Viewport.GetWorldSize();
		float min_world_span = (float)(std::min(world_size.x, world_size.y) - 1) * TerrainRenderer::MetersPerChunk / 4.0f;
		m_TerrainEditingPanel.RenderPanel(std::min(min_world_span, 512.0f));
	}
	else
	{
		m_TerrainEditingPanel.RenderPanel(512.0f);
	}

	m_Viewport.RenderPanel();

	m_NewTerrainModal.RenderModal();
	if (m_NewTerrainModal.IsDone())
	{
		if (m_Viewport.IsTerrainOpen())
			m_Viewport.CloseTerrain();

		const std::string& path = m_NewTerrainModal.GetPath();
		if (m_Viewport.FileNew(path, m_NewTerrainModal.GetWorldSize()))
		{
			m_TerrainPath = path;
			SDL_SetWindowTitle(li::Application::Get()->GetWindow()->GetWindow(), (m_TerrainPath.filename().string() + " - Horizons Level Editor").c_str());
		}
		else
		{
			SDL_SetWindowTitle(li::Application::Get()->GetWindow()->GetWindow(), "Horizons Level Editor");
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Horizons Level Editor", "Failed to open new terrain file.", li::Application::Get()->GetWindow()->GetWindow());
		}
	}
}

void LevelEditorLayer::OnEvent(SDL_Event* event)
{
	m_Viewport.OnEvent(event);

	if (event->type == SDL_KEYDOWN)
	{
		auto& input = li::Application::Get()->GetInput();
		if (input.IsKeyPressed(SDL_SCANCODE_LCTRL))
		{
			switch (event->key.keysym.scancode)
			{
			case SDL_SCANCODE_N:
				FileNew();
				break;
			case SDL_SCANCODE_O:
				FileOpen();
				break;
			case SDL_SCANCODE_S:
				if (input.IsKeyPressed(SDL_SCANCODE_LSHIFT))
					FileSaveAs();
				else
					FileSave();
				break;
			}
		}
	}
	else if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_CLOSE
		&& m_Viewport.IsTerrainOpen() && m_Viewport.IsTerrainModified())
	{
		int button_id;
		UnsavedChangesDialog(&button_id);
		// If open is cancelled.
		if (button_id < 0 || button_id >= 2)
			li::Application::Get()->EventHandled();
		// If save is clicked.
		if (button_id == 0)
			FileSave();
		// If don't save, ie. button_id == 1, do nothing.
	}
}

void LevelEditorLayer::FileNew()
{
	if (m_Viewport.IsTerrainOpen() && m_Viewport.IsTerrainModified())
	{
		int button_id;
		UnsavedChangesDialog(&button_id);
		// If open is cancelled.
		if (button_id < 0 || button_id >= 2)
			return;
		// If save is clicked.
		if (button_id == 0)
			FileSave();
		// If don't save, ie. button_id == 1, do nothing.
	}
	m_NewTerrainModal.Open();
}

void LevelEditorLayer::FileOpen()
{
	if (m_Viewport.IsTerrainOpen())
	{
		if (m_Viewport.IsTerrainModified())
		{
			int button_id;
			UnsavedChangesDialog(&button_id);
			// If open is cancelled.
			if (button_id < 0 || button_id >= 2)
				return;
			// If save is clicked.
			if (button_id == 0)
				FileSave();
			// If don't save, ie. button_id == 1, do nothing.
		}
		m_Viewport.CloseTerrain();
	}
	FileOpenDialog();
}

void LevelEditorLayer::FileOpenDialog()
{
	nfdchar_t* path;
	nfdresult_t result = NFD_OpenDialog("terrain", NULL, &path);
	if (result == NFD_OKAY)
	{
		if (m_Viewport.FileOpen(path))
		{
			m_TerrainPath = path;
			SDL_SetWindowTitle(li::Application::Get()->GetWindow()->GetWindow(), (m_TerrainPath.filename().string() + " - Horizons Level Editor").c_str());
			m_TitleHasAsterisk = false;
		}
		else
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Horizons Level Editor", "Failed to open terrain file.", li::Application::Get()->GetWindow()->GetWindow());
		}
	}
	else if (result == NFD_ERROR)
		LI_ERROR(NFD_GetError());
}

void LevelEditorLayer::FileSave()
{
	m_Viewport.FileSave();
}

void LevelEditorLayer::FileSaveAs()
{
	nfdchar_t* path;
	nfdresult_t result = NFD_SaveDialog("terrain", NULL, &path);
	if (result == NFD_OKAY)
		m_Viewport.FileSaveAs(path);
	else if (result == NFD_ERROR)
		LI_ERROR(NFD_GetError());
}

void LevelEditorLayer::UnsavedChangesDialog(int* button_id)
{
	std::string message = m_TerrainPath.filename().string() + " has unsaved changes. What would you like to do?";
	const SDL_MessageBoxData msg_box_data = {
		SDL_MESSAGEBOX_INFORMATION | SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT,
		li::Application::Get()->GetWindow()->GetWindow(),
		"Unsaved Changes",
		message.c_str(),
		SDL_arraysize(m_OverwriteButtons),
		m_OverwriteButtons,
		&m_MsgBoxColorScheme
	};

	if (SDL_ShowMessageBox(&msg_box_data, button_id) < 0) {
		LI_ERROR("Failed to show message box.");
		return;
	}
}
#endif