#pragma once
#ifndef LI_DIST
#include "Command.h"

#include "Lithium.h"

class Console
{
public:
	struct Line
	{
		std::string Text;
		glm::vec4 Color;
	};

	Console();

	inline void AddCommand(Li::Ref<Command> command)
	{
		LI_ASSERT(m_Commands.find(command->GetToken()) == m_Commands.end(), "Command already exists!");
		m_Commands.insert({ command->GetToken(), command });
	}

	void ExecuteCommand(const std::string& command);
	void WriteLine(const std::string& test, const glm::vec4& color);
	void Clear();

	const Line& GetLine(int index) const;
	inline int GetLineCount() const { return m_LineCount; }

	static constexpr int NumOutputLines = 64;

private:
	using CommandMap = std::unordered_map<std::string, Li::Ref<Command>>;
	CommandMap m_Commands;

	Line m_Lines[NumOutputLines];
	int m_LineCount;
	int m_CurrentLine;
};
#endif
