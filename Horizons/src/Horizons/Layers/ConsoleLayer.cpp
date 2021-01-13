#include "pch.h"
#ifndef LI_DIST
#include "ConsoleLayer.h"

#include "glm/glm.hpp"
#include "Horizons.h"


ConsoleLayer::ConsoleLayer()
	: Layer("Console"), m_HistoryBuffer(), m_ConsoleOpen(false), 
	m_NumHistoryItems(0), m_ScrollToBottom(false),
	m_HistoryIndex(-1), m_HistoryCount(0), m_LatestHistory(0)
{
	m_InputBuffer[0] = '\0';
}

void ConsoleLayer::OnUpdate(Li::Duration::us dt)
{
}

#ifndef LI_DIST
void ConsoleLayer::OnImGuiRender()
{
	// TODO: make m_ConsoleOpen a config variable.
	if (!m_ConsoleOpen)
		return;

	Console& console = Li::Application::Get<Horizons>().GetConsole();

	if (!ImGui::Begin("Console"))
	{
		ImGui::End();
		return;
	}

	ImGui::Separator();
	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
	ImGui::BeginChild("ScrollingRegion", { 0, -footer_height_to_reserve }, false, ImGuiWindowFlags_HorizontalScrollbar);

	for (int i = 0; i < console.GetLineCount(); i++)
	{
		const Console::Line& line = console.GetLine(i);

		ImGui::PushStyleColor(ImGuiCol_Text, { line.Color.r, line.Color.g, line.Color.b, line.Color.a });
		ImGui::TextUnformatted(line.Text.c_str());
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
	if (ImGui::InputText("Input", m_InputBuffer, InputBufferSize, flags, callback, this))
	{
		if (m_InputBuffer[0] != '\0')
		{
			console.ExecuteCommand(m_InputBuffer);
			AddHistory(m_InputBuffer);
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
		Li::Application::Get().TakeFocus("Console");
	else
		Li::Application::Get().ReleaseFocus();
}
#endif

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
				Li::Application::Get().ReleaseFocus();
			}
			
			Li::Application::Get().EventHandled();
		}
	}
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

void ConsoleLayer::AddHistory(const std::string& command)
{
	// Reset the history index back to the latest command.
	m_HistoryIndex = -1;

	// Add the command to the history buffer.
	m_HistoryBuffer[m_LatestHistory] = command;
	m_LatestHistory = (m_LatestHistory + 1) % InputHistoryCount;
	if (m_HistoryCount < InputHistoryCount)
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
			index += InputHistoryCount;

		strcpy_s(data->Buf, data->BufSize, m_HistoryBuffer[index].c_str());
		data->BufTextLen = (int)m_HistoryBuffer[index].size();
	}
	data->BufDirty = true;
	data->CursorPos = data->BufTextLen;
	data->SelectionStart = data->CursorPos;
	data->SelectionEnd = data->CursorPos;
}

#endif