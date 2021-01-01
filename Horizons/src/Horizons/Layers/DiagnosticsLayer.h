#pragma once
#ifndef LI_DIST
#include "Horizons/Core/Core.h"
#include "Lithium.h"

class DiagnosticsLayer : public Li::Layer
{
public:
	DiagnosticsLayer();
	virtual ~DiagnosticsLayer() = default;

	virtual void OnUpdate(Li::Duration::us dt) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(SDL_Event* event) override;

private:
	constexpr static int DiagnosticsMaxEntries = 2048;

	bool m_PerformanceOpen = false;
	bool m_CVarsOpen = false;

	Li::Duration::us m_CurrentDelta;

	Li::Timer m_Timer;

	float m_Average = 0.0f;
	Li::Duration::us m_AverageTotal;
	size_t m_AverageCount = 0ULL;

	Li::Duration::us m_Min;
	Li::Duration::us m_Max;

	float m_DisplayMin = 0.0f;
	float m_DisplayMax = 0.0f;

	float m_ScaleMin = 0.0f;
	float m_ScaleMax = 0.0015f;

	float m_Averages[DiagnosticsMaxEntries] = { 0.0f };
	float m_Minimums[DiagnosticsMaxEntries] = { 0.0f };
	float m_Maximums[DiagnosticsMaxEntries] = { 0.0f };
	int m_NumEntries = 0;
	int m_EntryIndex = 0;

	bool m_ShowAverave = true, m_ShowMin = false, m_ShowMax = false;
	bool m_ShowLive = true, m_ShowHistory = false;
};
#endif