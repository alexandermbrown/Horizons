#include "pch.h"
#ifndef LI_DIST
#include "TerrainEditingPanel.h"

#include "imgui.h"

TerrainEditingPanel::TerrainEditingPanel(Brush* brush)
	: m_WindowOpen(true), m_Brush(brush)
{
}

void TerrainEditingPanel::OnImGuiRender()
{
	if (m_WindowOpen)
	{
		if (ImGui::Begin("Terrain Editing", &m_WindowOpen))
		{
			ImGui::Separator();
			ImGui::Text("Brush Settings");
			ImGui::Checkbox("Brush Enabled", &m_Brush->Enabled);
			ImGui::Checkbox("Subtract", &m_Brush->Subtract);
			ImGui::SliderFloat("Strength", &m_Brush->Strength, 0.0f, 5.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
			ImGui::SliderFloat("Inner Radius", &m_Brush->InnerRadius, 0.0f, m_Brush->OuterRadius, "%.3f", ImGuiSliderFlags_Logarithmic);
			ImGui::SliderFloat("Outer Radius", &m_Brush->OuterRadius, m_Brush->InnerRadius, 512.0f, "%.3f", ImGuiSliderFlags_Logarithmic);

			ImGui::Separator();
			ImGui::Text("Layers");

		}
		ImGui::End();
	}
}
#endif
