#include "pch.h"
#include "ViewportPanel.h"

#include "Horizons/Rendering/RenderingComponents.h"
#include "Horizons/LevelEditor/EditorCameraSystem.h"
#include "imgui.h"

ViewportPanel::ViewportPanel()
	: m_BrushInnerRadius(0.5f), m_BrushOuterRadius(2.0f), m_BrushAmplitude(1.0f),
	m_ViewportOpen(true), m_ViewportSize(512, 256)
{
	m_ViewportFB = li::Framebuffer::Create(m_ViewportSize.x, m_ViewportSize.y);
	m_ViewportFB->SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });

	EditorCameraSystem::Init(m_Registry);
}

ViewportPanel::~ViewportPanel()
{
	EditorCameraSystem::Shutdown(m_Registry);
}

void ViewportPanel::OnUpdate(float dt)
{
	EditorCameraSystem::Update(m_Registry, dt);

	if (m_ViewportFB->GetSize() != m_ViewportSize)
	{
		m_ViewportFB->Resize(m_ViewportSize.x, m_ViewportSize.y);
		EditorCameraSystem::Resize(m_Registry, m_ViewportSize.x, m_ViewportSize.y);
	}

	if (m_ViewportOpen)
	{
		m_ViewportFB->Bind();
		m_ViewportFB->Clear();

		li::Renderer::BeginScene(m_Registry.ctx<cp::camera>().camera);
		li::Renderer::SubmitColored({ 0.4f, 0.5f, 0.7f, 1.0f }, glm::mat4(1.0f));
		li::Renderer::EndScene();
	}
}

void ViewportPanel::OnImGuiRender()
{
	if (m_ViewportOpen)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		if (ImGui::Begin("Viewport", &m_ViewportOpen))
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
