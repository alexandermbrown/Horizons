#include "pch.h"
#ifndef LI_DIST
#include "TerrainEditingPanel.h"

#include "imgui.h"

TerrainEditingPanel::TerrainEditingPanel(EditorSettings* settings)
	: m_WindowOpen(true), m_Settings(settings)
{
}

void TerrainEditingPanel::RenderPanel(float max_brush_radius)
{
	constexpr const char* const items[] = { "Show in Brush", "Show", "Hide" };

	if (m_WindowOpen)
	{
		if (ImGui::Begin("Terrain Editing", &m_WindowOpen))
		{
			ImGui::Separator();
			ImGui::Text("Viewport Settings");
			ImGui::Checkbox("Show chunk borders", &m_Settings->Display.ShowChunkBorders);
			ImGui::Combo("Vertex Display Mode", (int*)&m_Settings->Display.VertexDisplayMode, items, 3);

			ImGui::Separator();
			ImGui::Text("Brush Settings");
			ImGui::Checkbox("Brush Enabled", &m_Settings->Brush.Enabled);
			ImGui::Checkbox("Subtract", &m_Settings->Brush.Subtract);
			ImGui::SliderFloat("Strength", &m_Settings->Brush.Strength, 0.0f, 20.0f, "%.3f", ImGuiSliderFlags_Logarithmic);
			ImGui::SliderFloat("Inner Radius", &m_Settings->Brush.InnerRadius, 0.0f, m_Settings->Brush.OuterRadius, "%.3f");
			ImGui::SliderFloat("Outer Radius", &m_Settings->Brush.OuterRadius, glm::max(m_Settings->Brush.InnerRadius, 0.01f), max_brush_radius, "%.3f", ImGuiSliderFlags_Logarithmic);

			ImGui::Separator();
			ImGui::Text("Layer");
			ImGui::RadioButton("1", &m_Settings->Layer, 0); ImGui::SameLine();
			ImGui::RadioButton("2", &m_Settings->Layer, 1); ImGui::SameLine();
			ImGui::RadioButton("3", &m_Settings->Layer, 2);

			ImGui::Separator();
		}
		ImGui::End();
	}
}
#endif
