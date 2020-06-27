#include "pch.h"
#include "Command.h"

void Command::Execute(const std::string& args, std::string* errorOut)
{
	std::vector<CommandValue> arguments = std::vector<CommandValue>();

	if (m_Layout.GetCount() > 0)
	{
		size_t position = 0;
		while (position != std::string::npos)
		{
			size_t next_space = args.find(' ', position + 1);
			std::string current_arg = args.substr(position, next_space - position);
			position = next_space;

			// Check that there aren't too many parameters.
			if (arguments.size() >= m_Layout.GetCount())
			{
				*errorOut = "Too many arguments supplied. Command requires " + std::to_string(m_Layout.GetCount()) + " arguments.";
				return;
			}

			const CommandArgDefinition& argDef = m_Layout.GetArguments().at(arguments.size());

			CommandValue value(argDef.Type);
			switch (argDef.Type)
			{
			case CommandArgType::String:
				value.StringValue = current_arg;
				break;

			case CommandArgType::Boolean:
				if (current_arg == "true" || current_arg == "on" || current_arg == "t" || current_arg == "1")
				{
					value.BoolValue = true;
				}
				else if (current_arg == "false" || current_arg == "off" || current_arg == "f" || current_arg == "0")
				{
					value.BoolValue = false;
				}
				else
				{
					*errorOut = std::string("Argument ") + argDef.Name + " requires a boolean value.";
					return;
				}
				break;
				
			case CommandArgType::Integer:
				try
				{
					value.IntValue = std::stoi(current_arg);
				}
				catch (const std::invalid_argument&)
				{
					*errorOut = std::string("Argument ") + argDef.Name + " requires an integer value.";
					return;
				}
				catch (const std::out_of_range&)
				{
					*errorOut = std::string("Argument ") + argDef.Name + " integer out of range.";
					return;
				}
				break;

			case CommandArgType::Unsigned:
				try
				{
					value.UnsignedValue = std::stoul(current_arg);
				}
				catch (const std::invalid_argument&)
				{
					*errorOut = std::string("Argument ") + argDef.Name + " requires an unsigned integer value.";
					return;
				}
				catch (const std::out_of_range&)
				{
					*errorOut = std::string("Argument ") + argDef.Name + " unsigned integer out of range.";
					return;
				}
				break;

			case CommandArgType::Float:
				try
				{
					value.FloatValue = std::stof(current_arg);
				}
				catch (const std::invalid_argument&)
				{
					*errorOut = std::string("Argument ") + argDef.Name + " requires a floating point value.";
					return;
				}
				catch (const std::out_of_range&)
				{
					*errorOut = std::string("Argument ") + argDef.Name + " float out of range.";
					return;
				}
				break;

			default:
				*errorOut = std::string("Unknown argument type supplied for ") + argDef.Name;
				return;
			}
			arguments.push_back(value);
		}

		// Check that the user has supplied enough parameters.
		if (arguments.size() != m_Layout.GetCount())
		{
			*errorOut = "Not enough arguments supplied. Command requires " + std::to_string(m_Layout.GetCount()) + " arguments.";
			return;
		}
	}

	m_ExecuteCallback(std::move(arguments), errorOut);
}
