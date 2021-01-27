#include "pch.h"

#include "Horizons.h"
#include "Lithium.h"

#ifdef HZ_CONSOLE_ENABLED
#include "Horizons/Config/ConfigConvert.h"
#include "Horizons/Core/UserEvents.h"
#include "Horizons/Core/AppState.h"

#include "CVarCommands.h"

Li::Ref<Command> CreateCVarSetCommand()
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

	Li::Ref<Command> set = Li::MakeRef<Command>("set", "Sets the contents of a config variable.", layout,
		[](std::vector<CommandValue>&& args, std::string* error_out)
	{
		LI_ASSERT(args.size() == 2, "Incorrect number of arguments supplied to set command.");
		ConfigStore& store = Li::Application::Get<Horizons>().GetConfig();

		auto it = store.find(args[0].StringValue);
		if (it == store.end())
		{
			*error_out = "Config variable " + args[0].StringValue + " not found.";
			return;
		}

		if (!ConfigVariantSetFromString(it->second, args[1].StringValue))
		{
			*error_out = "Failed to set config variable " + args[0].StringValue + " to " + args[1].StringValue;
			return;
		}

		if (store.Get<int>("app_state") == static_cast<int>(AppState::InGame))
		{
			// Create event to change the variable in the game thread.
			SDL_Event event;
			SDL_zero(event);
			event.type = store.Get<int>("event_app_to_game");
			event.user.code = (uint32_t)UserEvent::SetConfig;

			event.user.data1 = new std::string(args[0].StringValue);

			if (std::holds_alternative<bool>(it->second))
				event.user.data2 = new bool(std::get<bool>(it->second));
			else if (std::holds_alternative<int>(it->second))
				event.user.data2 = new int(std::get<int>(it->second));
			else if (std::holds_alternative<float>(it->second))
				event.user.data2 = new float(std::get<float>(it->second));
			else if (std::holds_alternative<std::string>(it->second))
				event.user.data2 = new std::string(std::get<std::string>(it->second));
			else
				Li::Log::Error("Unknown config variable type!");

			SDL_PushEvent(&event);
			Li::Log::Trace("Pushed change event for console variable {}", args[0].StringValue);
		}
	});

	return set;
}

Li::Ref<Command> CreateCVarGetCommand()
{
	CommandLayout layout = {{
		"name",
		"The name of the cvar.",
		CommandArgType::String
	}};

	Li::Ref<Command> get = Li::MakeRef<Command>("get", "Gets the contents of a config variable.", layout,
		[](std::vector<CommandValue>&& args, std::string* error_out)
	{
		LI_ASSERT(args.size() == 1, "Incorrect number of arguments supplied to get command.");
		ConfigStore& store = Li::Application::Get<Horizons>().GetConfig();

		auto it = store.find(args[0].StringValue);
		if (it != store.end())
		{
			Li::Application::Get<Horizons>().GetConsole().WriteLine(
				VariantToString(it->second),
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			);
		}
		else *error_out = "Config variable " + args[0].StringValue + " not found.";
	});

	return get;
}
#endif
