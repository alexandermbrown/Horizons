#include "pch.h"
#ifndef LI_DIST
#include "DiagnosticsLayer.h"

#include "Horizons.h"

#include "imgui.h"

DiagnosticsLayer::DiagnosticsLayer()
	: m_Timer(0.1f, false, true)
{

}

void DiagnosticsLayer::OnAttach()
{

}

void DiagnosticsLayer::OnDetach()
{

}

void DiagnosticsLayer::OnUpdate(float dt)
{
	m_CurrentDelta = dt;

	m_AverageCount++;
	m_AverageTotal += dt;

	if (dt < m_Min)
		m_Min = dt;

	if (dt > m_Max)
		m_Max = dt;

	if (m_Timer.Update(dt))
	{
		m_Average = m_AverageTotal / (float)m_AverageCount;
		m_AverageTotal = 0.0f;
		m_AverageCount = 0ULL;

		m_DisplayMin = m_Min;
		m_DisplayMax = m_Max;

		m_Averages[m_EntryIndex] = m_Average;
		m_Maximums[m_EntryIndex] = m_Max;
		m_Minimums[m_EntryIndex] = m_Min;

		m_EntryIndex = (m_EntryIndex + 1) % DiagnosticsMaxEntries;

		m_Min = dt;
		m_Max = dt;

		if (m_NumEntries < DiagnosticsMaxEntries) m_NumEntries++;
	}
}

void DiagnosticsLayer::OnImGuiRender()
{
	if (m_PerformanceOpen)
	{
		ImGui::Begin("Performance", &m_PerformanceOpen);

		ImGui::Text(("Avg. FPS: " + std::to_string(1 / m_Average)).c_str());
		ImGui::Separator();

		ImGui::Text(("Avg. Frame Time: " + std::to_string(m_Average)).c_str());
		ImGui::Text(("Max. Frame Time: " + std::to_string(m_DisplayMax)).c_str());
		ImGui::Text(("Min. Frame Time: " + std::to_string(m_DisplayMin)).c_str());
		ImGui::Separator();

		ImGui::Text("Graph Settings");

		ImGui::Checkbox("Average", &m_ShowAverave);
		ImGui::SameLine();
		ImGui::Checkbox("Maximum", &m_ShowMax);
		ImGui::SameLine();
		ImGui::Checkbox("Minimum", &m_ShowMin);
		ImGui::SameLine();
		ImGui::Checkbox("History", &m_ShowHistory);
		ImGui::SameLine();
		ImGui::Checkbox("Live", &m_ShowLive); // TODO: FIX LIVE FRAME TIME //

		ImGui::Separator();

		ImGui::SliderFloat("Frame Time Max", &m_ScaleMax, m_ScaleMin + 0.0001f, 1.0f, "%.8f seconds", ImGuiSliderFlags_Logarithmic);
		ImGui::SliderFloat("Frame Time Min", &m_ScaleMin, 0.0f, m_ScaleMax - 0.0001f, "%.8f seconds", ImGuiSliderFlags_Logarithmic);
		ImGui::Separator();
		
		if (m_ShowAverave)
		{
			ImGui::Text("Average Frame Time");

			if (m_ShowHistory)
				ImGui::PlotLines("", m_Averages, m_NumEntries, 0, 0, m_ScaleMin, m_ScaleMax, { 300, 60 });

			if (m_ShowHistory && m_ShowLive)
				ImGui::SameLine();
			
			if (m_ShowLive)
				ImGui::PlotLines("", m_Averages + std::max(0, m_EntryIndex - 64), std::min(m_EntryIndex, 64), 0, 0, m_ScaleMin, m_ScaleMax, { 300, 60 });
		}
		
		if (m_ShowMax)
		{
			ImGui::Text("Maximum Frame Time");

			if (m_ShowHistory)
				ImGui::PlotLines("", m_Averages, m_NumEntries, 0, 0, m_ScaleMin, m_ScaleMax, { 300, 60 });

			if (m_ShowHistory && m_ShowLive)
				ImGui::SameLine();

			if (m_ShowLive)
				ImGui::PlotLines("", m_Maximums + std::max(0, m_EntryIndex - 64), std::min(m_EntryIndex, 64), 0, 0, m_ScaleMin, m_ScaleMax, { 300, 60 });
		}

		if (m_ShowMin)
		{
			ImGui::Text("Minimum Frame Time");

			if (m_ShowHistory)
				ImGui::PlotLines("", m_Averages, m_NumEntries, 0, 0, m_ScaleMin, m_ScaleMax, { 300, 60 });

			if (m_ShowHistory && m_ShowLive)
				ImGui::SameLine();

			if (m_ShowLive)
				ImGui::PlotLines("", m_Minimums + std::max(0, m_EntryIndex - 64), std::min(m_EntryIndex, 64), 0, 0, m_ScaleMin, m_ScaleMax, { 300, 60 });
		}

		ImGui::End();
	}

	if (m_CVarsOpen)
	{
		ImGui::Begin("Config Variables", &m_CVarsOpen);

		ConfigStore& config = li::Application::Get<Horizons>()->GetConfig();

		ImGui::BeginChild("cvar_list");
		for (auto& [name, var] : config)
		{
			ImGui::Text(name.c_str());
			ImGui::SameLine(ImGui::GetWindowWidth() - 50.0f);
			ImGui::Text(var.GetString().c_str());
		}
		ImGui::EndChild();

		ImGui::End();
	}
}

void DiagnosticsLayer::OnEvent(SDL_Event* event)
{
	if (event->type == SDL_KEYUP)
	{
		// Toggle console visibility.
		// TODO: Make scancode a keybind.
		if (event->key.keysym.scancode == SDL_SCANCODE_F3)
		{
			m_PerformanceOpen = !m_PerformanceOpen;

			li::Application::Get()->EventHandled();
		}
		else if (event->key.keysym.scancode == SDL_SCANCODE_F4)
		{
			m_CVarsOpen = !m_CVarsOpen;

			li::Application::Get()->EventHandled();
		}
	}
}
#endif