#pragma once

#ifndef LI_DIST
#include "Horizons/Core/Core.h"
#include "Horizons/Console/Command.h"

#include "Lithium.h"
#include "imgui.h"

class ConsoleLayer : public Li::Layer
{
public:
	ConsoleLayer();

	virtual void OnUpdate(Li::Duration::us dt) override;
#ifndef LI_DIST
	virtual void OnImGuiRender() override;
#endif
	virtual void OnEvent(SDL_Event* event) override;

	int InputTextCallback(ImGuiInputTextCallbackData* data);

private:
	static constexpr int InputBufferSize = 256;
	static constexpr int InputHistoryCount = 64;
	
	void AddHistory(const std::string& command);
	void UpdateHistory(ImGuiInputTextCallbackData* data);

	char m_InputBuffer[InputBufferSize];

	std::string m_HistoryBuffer[InputHistoryCount];
	int m_HistoryCount;
	int m_LatestHistory;
	int m_HistoryIndex;
	
	int m_NumHistoryItems;

	bool m_ConsoleOpen;

	bool m_ScrollToBottom;
};
#endif
