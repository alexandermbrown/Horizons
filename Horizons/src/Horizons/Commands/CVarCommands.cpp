#include "pch.h"
#include "CVarCommands.h"

#include "Horizons.h"
#include "Lithium.h"

li::Ref<Command> CreateCVarSetCommand()
{
	CommandLayout layout = {
		{
		"name",
		"The name of the cvar.",
		CommandArgType::String
		},
		{
		"value",
		"The value to set the cvar to",
		CommandArgType::String
		}
	};

	li::Ref<Command> set = li::CreateRef<Command>("set", "Sets the contents of a cvar.", layout, [](std::vector<CommandValue>&& args, std::string* errorOut)
		{
			LI_ASSERT(args.size() == 2, "Incorrect number of arguments supplied to set command.");
			ConfigStore& store = li::Application::Get<Horizons>()->GetConfig();
			if (store.Contains(args[0].StringValue))
			{
				if (!store.Get(args[0].StringValue).SetFromString(args[1].StringValue))
				{
					*errorOut = "Failed to set CVar " + args[0].StringValue + " to " + args[1].StringValue;
				}
			}
			else
			{
				*errorOut = "CVar " + args[0].StringValue + " not found.";
			}
		});

	return set;
}

li::Ref<Command> CreateCVarGetCommand()
{
	CommandLayout layout = {
		{
		"name",
		"The name of the cvar.",
		CommandArgType::String
		}
	};

	li::Ref<Command> get = li::CreateRef<Command>("get", "Gets the contents of a cvar.", layout, [](std::vector<CommandValue>&& args, std::string* errorOut)
		{
			LI_ASSERT(args.size() == 1, "Incorrect number of arguments supplied to get command.");
			ConfigStore& store = li::Application::Get<Horizons>()->GetConfig();
			if (store.Contains(args[0].StringValue))
			{
				li::Application::Get<Horizons>()->GetConsole()->Print( {
						store.Get(args[0].StringValue).GetString(),
						{1.0f, 1.0f, 1.0f, 1.0f}
					} );
			}
			else
			{
				*errorOut = "CVar " + args[0].StringValue + " not found.";
			}
		});

	return get;
}
