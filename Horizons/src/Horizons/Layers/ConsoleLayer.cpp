#include "pch.h"
#include "ConsoleLayer.h"

#ifdef HZ_CONSOLE_ENABLED

#include "glm/glm.hpp"
#include "Horizons.h"


ConsoleLayer::ConsoleLayer()
	: m_Commands(), m_InputBuffer(), m_HistoryBuffer(), m_ConsoleOpen(false), 
	m_NumHistoryItems(0), m_Lines(), m_LineCount(0), m_CurrentLine(0), m_ScrollToBottom(false),
	m_HistoryIndex(-1), m_HistoryCount(0), m_LatestHistory(0)
{
	m_InputBuffer[0] = '\0';

	CommandLayout clear_layout = {};

	AddCommand(li::CreateRef<Command>("clear", "Clears the console.", clear_layout, [](std::vector<CommandValue>&& args, std::string* errorOut)
		{
			li::Application::Get<Horizons>()->GetConsole()->Clear();
		}));
}

void ConsoleLayer::OnAttach()
{

}

void ConsoleLayer::OnDetach()
{

}

void ConsoleLayer::OnUpdate(float dt)
{

}

void ConsoleLayer::OnImGuiRender()
{
	// TODO: make m_ConsoleOpen a config variable.
	if (m_ConsoleOpen)
	{
		if (!ImGui::Begin("Console"))
		{
			ImGui::End();
			return;
		}

		ImGui::Separator();
		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

		for (int i = 0; i < m_LineCount; i++)
		{
			int current_index = m_LineCount == HZ_CONSOLE_OUTPUT_NUM_LINES ?
				current_index = (m_CurrentLine + i) % HZ_CONSOLE_OUTPUT_NUM_LINES : i;


			const glm::vec4& color = m_Lines[current_index].Color;
			ImGui::PushStyleColor(ImGuiCol_Text, { color.r, color.g, color.b, color.a });

			ImGui::TextUnformatted(m_Lines[current_index].Text.c_str());

			ImGui::PopStyleColor();

			if (m_ScrollToBottom)
				ImGui::SetScrollHereY(0.999f);
			m_ScrollToBottom = false;
		}

		ImGui::EndChild();
		ImGui::Separator();

		bool reclaimFocus = false;

		constexpr ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
		constexpr auto callback = [](ImGuiTextEditCallbackData* data) -> int { return ((ConsoleLayer*)data->UserData)->InputTextCallback(data); };
		if (ImGui::InputText("Input", m_InputBuffer, HZ_CONSOLE_INPUT_BUFFER_SIZE, flags, callback, this))
		{
			if (m_InputBuffer[0] != '\0')
			{
				std::string command = m_InputBuffer;
				ExecuteCommand(command);
				AddHistory(command);
				m_ScrollToBottom = true;
			}

			m_InputBuffer[0] = '\0';

			reclaimFocus = true;
		}

		ImGui::SetItemDefaultFocus();
		if (reclaimFocus)
			ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

		ImGui::End();

		if (ImGui::GetIO().WantCaptureKeyboard)
			li::Application::Get()->TakeFocus(this);
		else
			li::Application::Get()->TakeFocus(nullptr);
	}
}

void ConsoleLayer::OnEvent(SDL_Event* event)
{
	if (event->type == SDL_KEYUP)
	{
		// Toggle console visibility.
		// TODO: Make scancode a keybind.
		if (event->key.keysym.scancode == SDL_SCANCODE_GRAVE)
		{
			m_ConsoleOpen = !m_ConsoleOpen;
			if (m_ConsoleOpen)
			{
				m_ScrollToBottom = true;
			}
			else
			{
				li::Application::Get()->TakeFocus(nullptr);
			}
			
			li::Application::Get()->EventHandled();
		}
	}
}

void ConsoleLayer::Print(const Line& line)
{
	m_Lines[m_CurrentLine] = line;
	m_CurrentLine = (m_CurrentLine + 1) % HZ_CONSOLE_OUTPUT_NUM_LINES;
	if (m_LineCount < HZ_CONSOLE_OUTPUT_NUM_LINES)
		m_LineCount++;
}

void ConsoleLayer::Clear()
{
	m_CurrentLine = 0;
	m_LineCount = 0;
}

int ConsoleLayer::InputTextCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackCompletion)
	{
		// TODO: autocomplete.
	}
	else if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory)
	{
		if (data->EventKey == ImGuiKey_UpArrow)
		{
			if (m_HistoryIndex + 1 < m_HistoryCount)
			{
				m_HistoryIndex++;
				UpdateHistory(data);
			}
		}
		else if (data->EventKey == ImGuiKey_DownArrow)
		{
			if (m_HistoryIndex >= 0)
			{
				m_HistoryIndex--;
				UpdateHistory(data);
			}
		}
	}

	return 0;
}

void ConsoleLayer::ExecuteCommand(const std::string& command)
{
	Print({ "$ " + command, { 1.0f, 1.0f, 1.0f, 1.0f } });

	size_t first_space = command.find(" ");

	std::string token = command.substr(0, first_space);

	if (m_Commands.find(token) != m_Commands.end())
	{
		std::string error = "";
		m_Commands.at(token)->Execute(command.substr(first_space + 1), &error);

		if (!error.empty())
		{
			LI_ERROR(error);
			Print({ "[error] " + error, { 1.0f, 0.1f, 0.1f, 1.0f } });
		}
	}
	else Print({ "[error] Unknown command " + token, { 1.0f, 0.1f, 0.1f, 1.0f } });
}

void ConsoleLayer::AddHistory(const std::string& command)
{
	// Reset the history index back to the latest command.
	m_HistoryIndex = -1;

	// Add the command to the history buffer.
	m_HistoryBuffer[m_LatestHistory] = command;
	m_LatestHistory = (m_LatestHistory + 1) % HZ_CONSOLE_INPUT_HISTORY_COUNT;
	if (m_HistoryCount < HZ_CONSOLE_INPUT_HISTORY_COUNT)
		m_HistoryCount++;

}

void ConsoleLayer::UpdateHistory(ImGuiInputTextCallbackData* data)
{
	if (m_HistoryIndex < 0)
	{
		data->Buf[0] = '\0';
		data->BufTextLen = 0;
	}
	else
	{
		int index = m_LatestHistory - m_HistoryIndex - 1;

		while (index < 0)
			index += HZ_CONSOLE_INPUT_HISTORY_COUNT;

		strcpy_s(data->Buf, data->BufSize, m_HistoryBuffer[index].c_str());
		data->BufTextLen = m_HistoryBuffer[index].size();
	}
	data->BufDirty = true;
	data->CursorPos = data->BufTextLen;
	data->SelectionStart = data->CursorPos;
	data->SelectionEnd = data->CursorPos;
}

#endif