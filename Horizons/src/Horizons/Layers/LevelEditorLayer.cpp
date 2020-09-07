#include "pch.h"
#ifndef LI_DIST
#include "LevelEditorLayer.h"

#include "imgui.h"

LevelEditorLayer::LevelEditorLayer()
	: m_ReturnToMainMenu(false), m_BrushInnerRadius(0.5f), m_BrushOuterRadius(2.0f), m_BrushAmplitude(1.0f),
	m_DockspaceOpen(true), m_ViewportOpen(true), m_ViewportSize(512, 256)
{
	m_ViewportFB = li::Framebuffer::Create(m_ViewportSize.x, m_ViewportSize.y);
	m_ViewportFB->SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
}

LevelEditorLayer::~LevelEditorLayer()
{

}

void LevelEditorLayer::OnAttach()
{
	
}

void LevelEditorLayer::OnDetach()
{

}

void LevelEditorLayer::OnUpdate(float dt)
{
	if (m_ViewportFB->GetSize() != m_ViewportSize)
	{
		m_ViewportFB->Resize(m_ViewportSize.x, m_ViewportSize.y);
	}

	if (m_ViewportOpen)
	{
		m_ViewportFB->Bind();
		m_ViewportFB->Clear();
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
			if (ImGui::MenuItem("New", "Ctrl+N")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O")) {}
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
			if (ImGui::MenuItem("Viewport")) m_ViewportOpen = true;


			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	ImGui::End();

	// Viewport
	if (m_ViewportOpen)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		if (ImGui::Begin("Viewport", &m_ViewportOpen))
		{
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			
			ImGui::Image(m_ViewportFB->GetTexture()->GetInternalTexture(), viewportPanelSize, { 0, 1 }, { 1, 0 });
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}
}

void LevelEditorLayer::OnEvent(SDL_Event* event)
{

}
#endif