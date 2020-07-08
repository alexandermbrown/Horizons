#pragma once

#include "Horizons/Core/Core.h"
#include "Lithium.h"

#define HZ_DIAGNOSTICS_MAX_ENTRIES 2048

class DiagnosticsLayer : public li::Layer
{
public:
	DiagnosticsLayer();
	virtual ~DiagnosticsLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(float dt) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(SDL_Event* event) override;

private:

	bool m_PerformanceOpen = false;
	bool m_CVarsOpen = false;

	float m_CurrentDelta = 0.0f;

	li::Timer<float> m_Timer;

	float m_Average = 0.0f;
	float m_AverageTotal = 0.0f;
	size_t m_AverageCount = 0ULL;

	float m_Min = 0.0f;
	float m_Max = 0.0f;

	float m_DisplayMin = 0.0f;
	float m_DisplayMax = 0.0f;

	float m_ScaleMin = 0.0f;
	float m_ScaleMax = 0.0015f;

	float m_Averages[HZ_DIAGNOSTICS_MAX_ENTRIES] = { 0.0f };
	float m_Minimums[HZ_DIAGNOSTICS_MAX_ENTRIES] = { 0.0f };
	float m_Maximums[HZ_DIAGNOSTICS_MAX_ENTRIES] = { 0.0f };
	int m_NumEntries = 0;
	int m_EntryIndex = 0;

	bool m_ShowAverave = true, m_ShowMin = false, m_ShowMax = false;
	bool m_ShowLive = true, m_ShowHistory = false;
};