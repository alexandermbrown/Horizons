#pragma once

#include "Lithium.h"
#include "imgui.h"

#include "Horizons/Commands/Command.h"

#define HZ_CONSOLE_INPUT_BUFFER_SIZE 256
#define HZ_CONSOLE_INPUT_HISTORY_COUNT 64
#define HZ_CONSOLE_OUTPUT_NUM_LINES 64

struct Line
{
	Line()
		: Text(), Color(0.0f) {}
	Line(const std::string& text, const glm::vec4& color)
		: Text(text), Color(color) {}

	std::string Text;
	glm::vec4 Color;
};

class ConsoleLayer : public li::Layer
{
public:
	ConsoleLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(float dt) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(SDL_Event* event) override;

	inline void AddCommand(li::Ref<Command> command)
	{
		LI_ASSERT(m_Commands.find(command->GetToken()) == m_Commands.end(), "Command already exists!");
		m_Commands.insert({ command->GetToken(), command });
	}

	void Print(const Line& line);
	void Clear();

	int InputTextCallback(ImGuiInputTextCallbackData* data);

private:

	void ExecuteCommand(const std::string& command);
	void AddHistory(const std::string& command);
	void UpdateHistory(ImGuiInputTextCallbackData* data);

	std::unordered_map<std::string, li::Ref<Command>> m_Commands;

	char m_InputBuffer[HZ_CONSOLE_INPUT_BUFFER_SIZE];

	std::string m_HistoryBuffer[HZ_CONSOLE_INPUT_HISTORY_COUNT];
	int m_HistoryCount;
	int m_LatestHistory;
	int m_HistoryIndex;

	Line m_Lines[HZ_CONSOLE_OUTPUT_NUM_LINES];
	int m_LineCount;
	int m_CurrentLine;

	bool m_ConsoleOpen;
	int m_NumHistoryItems;

	bool m_ScrollToBottom;
};