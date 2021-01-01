#include "pch.h"
#ifndef LI_DIST
#include "Console.h"

#include "Horizons.h"

Console::Console()
	: m_Lines(), m_LineCount(0), m_CurrentLine(0)
{
	CommandLayout clear_layout = {};
	AddCommand(Li::MakeRef<Command>("clear", "Clears the console.", clear_layout, [](std::vector<CommandValue>&& args, std::string* error_out)
	{
		Li::Application::Get<Horizons>().GetConsole().Clear();
	}));
}

void Console::ExecuteCommand(const std::string& command)
{
	WriteLine("$ " + command, { 1.0f, 1.0f, 1.0f, 1.0f });

	size_t first_space = command.find(" ");

	std::string token = command.substr(0, first_space);

	CommandMap::iterator iter = m_Commands.find(token);
	if (iter != m_Commands.end())
	{
		std::string error;
		iter->second->Execute(command.substr(first_space + 1), &error);

		if (!error.empty())
		{
			LI_ERROR(error);
			WriteLine("[error] " + error, { 1.0f, 0.1f, 0.1f, 1.0f });
		}
	}
	else WriteLine("[error] Unknown command " + token, { 1.0f, 0.1f, 0.1f, 1.0f });
}

void Console::WriteLine(const std::string& text, const glm::vec4& color)
{
	m_Lines[m_CurrentLine].Text = text;
	m_Lines[m_CurrentLine].Color = color;
	m_CurrentLine = (m_CurrentLine + 1) % NumOutputLines;
	if (m_LineCount < NumOutputLines)
		m_LineCount++;
}

void Console::Clear()
{
	m_CurrentLine = 0;
	m_LineCount = 0;
}

const Console::Line& Console::GetLine(int index) const
{
	if (m_LineCount == NumOutputLines)
		return m_Lines[(m_CurrentLine + index) % NumOutputLines];
	else
		return m_Lines[index];
}
#endif
