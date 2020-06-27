#pragma once

#include <string>
#include <functional>
#include <vector>

enum class CommandArgType
{
	None = 0,
	String,
	Boolean,
	Integer,
	Unsigned,
	Float
};

struct CommandArgDefinition
{
	const char* Name;
	const char* Description;
	CommandArgType Type;
};

class CommandLayout
{
public:

	CommandLayout(const std::initializer_list<CommandArgDefinition>& arguments)
		: m_Arguments(arguments) {}

	inline const std::vector<CommandArgDefinition>& GetArguments() const { return m_Arguments; }
	inline const size_t GetCount() const { return m_Arguments.size(); }

	std::vector<CommandArgDefinition>::iterator begin() { return m_Arguments.begin(); }
	std::vector<CommandArgDefinition>::iterator end() { return m_Arguments.end(); }
	std::vector<CommandArgDefinition>::const_iterator begin() const { return m_Arguments.begin(); }
	std::vector<CommandArgDefinition>::const_iterator end() const { return m_Arguments.end(); }

private:
	std::vector<CommandArgDefinition> m_Arguments;
};

struct CommandValue
{
	CommandValue() : Type(CommandArgType::None), IntValue(0) {}
	CommandValue(CommandArgType type) : Type(type)
	{
		switch (Type)
		{
		case CommandArgType::String:
			StringValue = std::string();
			break;
		case CommandArgType::Boolean:
			BoolValue = false;
			break;
		case CommandArgType::Integer:
			IntValue = 0;
			break;
		case CommandArgType::Unsigned:
			UnsignedValue = 0;
			break;
		case CommandArgType::Float:
			break;
			FloatValue = 0.0f;
		default:
			IntValue = 0;
		}
	}

	CommandValue(const CommandValue& c)
	{
		Type = c.Type;
		switch (Type)
		{
		case CommandArgType::String:
			StringValue = c.StringValue;
			break;
		case CommandArgType::Boolean:
			BoolValue = c.BoolValue;
			break;
		case CommandArgType::Integer:
			IntValue = c.IntValue;
			break;
		case CommandArgType::Unsigned:
			UnsignedValue = c.UnsignedValue;
			break;
		case CommandArgType::Float:
			break;
			FloatValue = c.FloatValue;
		default:
			IntValue = 0;
		}
	}
	~CommandValue() {}

	CommandArgType Type;
	std::string StringValue;

	union
	{
		bool BoolValue;
		int IntValue;
		unsigned int UnsignedValue;
		float FloatValue;
	};
};

using CommandCallbackFn = std::function<void(std::vector<CommandValue>&& args, std::string * errorOut)>;

class Command
{
public:
	
	Command(const std::string& token,
		const std::string& helpMsg,
		const CommandLayout& layout,
		const CommandCallbackFn& executeCallback)
		: m_Token(token), m_HelpMsg(helpMsg), m_Layout(layout), m_ExecuteCallback(executeCallback) {}

	void Execute(const std::string& args, std::string* errorOut);

	inline const std::string& GetToken() const { return m_Token; }
	inline const std::string& GetHelpMsg() const { return m_HelpMsg; }
	inline const CommandLayout& GetLayout() const { return m_Layout; }

private:
	std::string m_Token;
	std::string m_HelpMsg;
	CommandLayout m_Layout;

	CommandCallbackFn m_ExecuteCallback;
};