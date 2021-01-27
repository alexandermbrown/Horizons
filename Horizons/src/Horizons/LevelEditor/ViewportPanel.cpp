#include "pch.h"
#ifndef LI_DIST
#include "ViewportPanel.h"

#include "EditorComponents.h"
#include "BrushSystem.h"
#include "Horizons/Rendering/RenderingComponents.h"
#include "Horizons/LevelEditor/EditorCameraSystem.h"
#include "TerrainVerticeSystem.h"
#include "ChunkBorderSystem.h"

#include "imgui.h"

ViewportPanel::ViewportPanel(EditorSettings* settings)
	: m_Settings(settings), m_WindowOpen(true), m_ViewportSize(512, 256), m_TerrainOpen(false),
		m_TerrainRenderer(&m_TerrainStore, 7)
{
	m_ViewportFB = Li::Framebuffer::Create(m_ViewportSize.x, m_ViewportSize.y);
	m_ViewportFB->SetClearColor({ 0.25f, 0.25f, 0.25f, 1.0f });

	EditorCameraSystem::Init(m_Registry);

	m_Registry.set<cp::brush>(&settings->Brush);
}

ViewportPanel::~ViewportPanel()
{
	EditorCameraSystem::Shutdown(m_Registry);
	if (m_TerrainOpen)
	{
		m_TerrainRenderer.UnloadTerrain();
	}
}

void ViewportPanel::OnUpdate(Li::Duration::us dt)
{
	EditorCameraSystem::Update(m_Registry, dt);

	if (m_TerrainOpen)
	{
		m_CameraFocus = EditorCameraSystem::GetCameraFocusPoint(m_Registry);
		m_ChunkCenter = {
			(int)std::floor(m_CameraFocus.x / TerrainRenderer::MetersPerChunk),
			(int)std::floor(m_CameraFocus.y / TerrainRenderer::MetersPerChunk)
		};
		m_TerrainRenderer.UpdateCenter(m_ChunkCenter);
	}

	if (m_ViewportFB->GetSize() != m_ViewportSize)
	{
		m_ViewportFB->Resize(m_ViewportSize.x, m_ViewportSize.y);
		EditorCameraSystem::Resize(m_Registry, m_ViewportSize.x, m_ViewportSize.y);
	}

	if (m_ViewportHovered)
	{
		int mouse_x, mouse_y;
		if (SDL_GetMouseState(&mouse_x, &mouse_y) & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			auto& brush = m_Registry.ctx<cp::brush>();
			m_TerrainStore.ApplyBrush(brush.brush, brush.world_pos, m_Settings->Layer, dt);
		}
	}

	if (m_WindowOpen && m_TerrainOpen)
	{
		m_TerrainRenderer.RenderFramebuffer();

		m_ViewportFB->Bind();
		m_ViewportFB->Clear();

		Li::Renderer::BeginScene(m_Registry.ctx<cp::camera>().camera);
		m_TerrainRenderer.SubmitQuad();

		switch (m_Settings->Display.VertexDisplayMode)
		{
		case VertexDisplayMode::Show:
			TerrainVerticeSystem::SubmitVerticesShow(m_Registry, m_ChunkCenter, m_TerrainRenderer.RenderWidth);
			break;
		case VertexDisplayMode::ShowInBrush:
			TerrainVerticeSystem::SubmitVerticesShowInBrush(m_Registry, m_ChunkCenter, m_TerrainRenderer.RenderWidth);
		}

		if (m_Settings->Display.ShowChunkBorders)
		{
			ChunkBorderSystem::SubmitBorders(m_Registry, m_CameraFocus, m_TerrainStore.GetWorldSize());
		}

		BrushSystem::SubmitBrush(m_Registry, m_MousePos, m_ViewportFB->GetSize());

		Li::Renderer::EndScene();
	}
}

void ViewportPanel::RenderPanel()
{
	if (m_WindowOpen)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		if (ImGui::Begin("Viewport", &m_WindowOpen))
		{
			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			
			ImVec2 mouse_pos = ImGui::GetMousePos();
			ImVec2 cursor_pos = ImGui::GetCursorScreenPos();

			m_MousePos = { mouse_pos.x - cursor_pos.x, mouse_pos.y - cursor_pos.y };

			ImVec2 viewport_size = ImGui::GetContentRegionAvail();
			m_ViewportSize = { viewport_size.x, viewport_size.y };
			
			// Account for OpenGL's flipped framebuffer texture.
			if (Li::Application::Get().GetAPI() == Li::RendererAPI::OpenGL)
				ImGui::Image(m_ViewportFB->GetTexture()->GetInternalTexture(), viewport_size, { 0, 1 }, { 1, 0 });
			else
				ImGui::Image(m_ViewportFB->GetTexture()->GetInternalTexture(), viewport_size, { 0, 0 }, { 1, 1 });
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}
}

void ViewportPanel::OnEvent(SDL_Event* event)
{
	if (m_ViewportHovered)
	{
		EditorCameraSystem::OnEvent(m_Registry, event, m_ViewportFB->GetSize());
	}
}

bool ViewportPanel::FileNew(const std::string& path, glm::ivec2 world_size)
{
	if (!m_TerrainStore.CreateEmptyTerrainFile(path, world_size))
		return false;

	return FileOpen(path);
}

bool ViewportPanel::FileOpen(const std::string& path)
{
	LI_ASSERT(!m_TerrainOpen, "Terrain must not be open.");
	Li::Log::Trace("Opening terrain file {}", path);
	m_CameraFocus = EditorCameraSystem::GetCameraFocusPoint(m_Registry);
	
	m_TerrainOpen = m_TerrainRenderer.LoadTerrain(path, {
		(int)std::floor(m_CameraFocus.x / TerrainRenderer::MetersPerChunk),
		(int)std::floor(m_CameraFocus.y / TerrainRenderer::MetersPerChunk)
	});

	return m_TerrainOpen;
}

void ViewportPanel::FileSave()
{
	m_TerrainStore.Save();
}

void ViewportPanel::FileSaveAs(const std::string& path)
{
	// TODO: handle error in editor layer.
	if (!m_TerrainStore.SaveAs(path))
	{
		Li::Log::Error("Failed to save terrain as {}", path);
	}
}

void ViewportPanel::CloseTerrain()
{
	m_TerrainStore.UnloadTerrain();
	m_TerrainOpen = false;
}
#endif
