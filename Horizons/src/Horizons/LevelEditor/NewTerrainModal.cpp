#include "pch.h"
#ifndef LI_DIST
#include "NewTerrainModal.h"

#include "Lithium.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "nfd.h"

#include <filesystem>
#include <cmath>

NewTerrainModal::NewTerrainModal()
	: m_IsDone(false), m_ImGuiOpenPopup(false), m_WorldSize(32, 32)
{
}

void NewTerrainModal::RenderModal()
{
	if (m_ImGuiOpenPopup)
	{
		ImGui::OpenPopup(m_WindowTitle);
		m_ImGuiOpenPopup = false;
	}

	bool unused_open;
	if (ImGui::BeginPopupModal(m_WindowTitle, &unused_open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
	{
		ImGui::TextUnformatted("World Size");
		if (ImGui::InputInt2("width by height in chunks", glm::value_ptr(m_WorldSize)))
		{
			if (m_WorldSize.x < 1)
				m_WorldSize.x = 1;
			if (m_WorldSize.y < 1)
				m_WorldSize.y = 1;
		}
		ImGui::TextUnformatted("Path");
		if (ImGui::Button("Browse...")) FileDialog();
		ImGui::SameLine();
		ImGui::PushItemWidth(std::max(400.0f, 7.5f * m_Path.length()));
		ImGui::InputText("", &m_Path);
		ImGui::PopItemWidth();

		ImGui::NewLine();

		if (ImGui::Button("Create", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			m_IsDone = true;
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}
}

bool NewTerrainModal::IsDone()
{
	if (m_IsDone)
	{
		m_IsDone = false;
		return true;
	}
	return false;
}

void NewTerrainModal::FileDialog()
{
	nfdchar_t* path;
	nfdresult_t result = NFD_SaveDialog("terrain", NULL, &path);
	if (result == NFD_OKAY)
	{
		std::filesystem::path fs_path = path;
		if (fs_path.has_extension())
			m_Path = fs_path.string();
		else
			m_Path = fs_path.replace_extension(".terrain").string();
	}
	else if (result == NFD_ERROR)
		LI_ERROR(NFD_GetError());
}

#endif
