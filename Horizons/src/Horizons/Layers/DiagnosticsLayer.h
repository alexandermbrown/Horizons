#pragma once
#ifndef LI_DIST
#include "Horizons/Core/Core.h"
#include "Lithium.h"

class DiagnosticsLayer : public li::Layer
{
public:
	DiagnosticsLayer();
	virtual ~DiagnosticsLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(li::duration::us dt) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(SDL_Event* event) override;

private:
	constexpr static int DiagnosticsMaxEntries = 2048;

	bool m_PerformanceOpen = false;
	bool m_CVarsOpen = false;

	li::duration::us m_CurrentDelta;

	li::Timer m_Timer;

	float m_Average = 0.0f;
	li::duration::us m_AverageTotal;
	size_t m_AverageCount = 0ULL;

	li::duration::us m_Min;
	li::duration::us m_Max;

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