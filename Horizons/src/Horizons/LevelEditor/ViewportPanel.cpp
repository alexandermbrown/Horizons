#include "pch.h"
#include "ViewportPanel.h"

#include "Horizons/Rendering/RenderingComponents.h"
#include "Horizons/LevelEditor/EditorCameraSystem.h"
#include "imgui.h"

#include <filesystem>

ViewportPanel::ViewportPanel()
	: m_WindowOpen(true), m_ViewportSize(512, 256), m_TerrainOpen(false),
	m_TerrainStore(), m_TerrainRenderer(&m_TerrainStore, 7)
{
	m_ViewportFB = li::Framebuffer::Create(m_ViewportSize.x, m_ViewportSize.y);
	m_ViewportFB->SetClearColor({ 0.25f, 0.25f, 0.25f, 1.0f });

	EditorCameraSystem::Init(m_Registry);
}

ViewportPanel::~ViewportPanel()
{
	EditorCameraSystem::Shutdown(m_Registry);
	if (m_TerrainOpen)
	{
		m_TerrainRenderer.UnloadTerrain();
	}
}

void ViewportPanel::OnUpdate(float dt)
{
	EditorCameraSystem::Update(m_Registry, dt);

	if (m_TerrainOpen)
	{
		auto& camera_pos = EditorCameraSystem::GetCameraFocusPoint(m_Registry);
		m_TerrainRenderer.UpdateCenter({
			(int)std::floor(camera_pos.x / TerrainRenderer::MetersPerChunk),
			(int)std::floor(camera_pos.y / TerrainRenderer::MetersPerChunk)
		});
	}

	if (m_ViewportFB->GetSize() != m_ViewportSize)
	{
		m_ViewportFB->Resize(m_ViewportSize.x, m_ViewportSize.y);
		EditorCameraSystem::Resize(m_Registry, m_ViewportSize.x, m_ViewportSize.y);
	}

	if (m_WindowOpen && m_TerrainOpen)
	{
		m_TerrainRenderer.RenderFramebuffer();

		m_ViewportFB->Bind();
		m_ViewportFB->Clear();

		li::Renderer::BeginScene(m_Registry.ctx<cp::camera>().camera);
		m_TerrainRenderer.SubmitQuad();
		li::Renderer::EndScene();

	}
}

void ViewportPanel::OnImGuiRender()
{
	if (m_WindowOpen)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		if (ImGui::Begin("Viewport", &m_WindowOpen))
		{
			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			// Account for OpenGL's flipped framebuffer texture.
			if (li::Application::Get()->GetAPI() == li::RendererAPI::OpenGL)
				ImGui::Image(m_ViewportFB->GetTexture()->GetInternalTexture(), viewportPanelSize, { 0, 1 }, { 1, 0 });
			else
				ImGui::Image(m_ViewportFB->GetTexture()->GetInternalTexture(), viewportPanelSize, { 0, 0 }, { 1, 1 });
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}
}

void ViewportPanel::OnEvent(SDL_Event* event)
{
	if (m_ViewportFocused && m_ViewportHovered)
	{
		EditorCameraSystem::OnEvent(m_Registry, event, m_ViewportFB->GetSize());
	}
}

void ViewportPanel::FileOpen(const std::string& path)
{
	LI_TRACE("Opening terrain file {}", path);
	m_TerrainOpen = true;
	auto& pos = EditorCameraSystem::GetCameraFocusPoint(m_Registry);
	
	if (m_TerrainRenderer.LoadTerrain(path, {
		(int)std::floor(pos.x / TerrainRenderer::MetersPerChunk),
		(int)std::floor(pos.y / TerrainRenderer::MetersPerChunk)
		}))
	{
		std::filesystem::path sys_path = path;
		SDL_SetWindowTitle(li::Application::Get()->GetWindow()->GetWindow(), ("Horizons Level Editor - " + sys_path.filename().string()).c_str());
	}
	else
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Horizons Level Editor", "Failed to open terrain file.", li::Application::Get()->GetWindow()->GetWindow());
	}

}
