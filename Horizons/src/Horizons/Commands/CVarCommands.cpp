#include "pch.h"

#include "Horizons.h"
#include "Lithium.h"

#include "Horizons/Core/UserEvents.h"
#include "Horizons/Core/AppState.h"

#ifdef HZ_CONSOLE_ENABLED
#include "CVarCommands.h"

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
				auto& var = store.Get(args[0].StringValue);
				if (var.SetFromString(args[1].StringValue))
				{
					if (store.Get("app_state").GetUnsigned() == (uint32_t)AppState::InGame)
					{
						// Create event to change the variable in the game thread.
						SDL_Event event;
						SDL_zero(event);
						event.type = store.Get("event_app_to_game").GetUnsigned();
						event.user.code = (uint32_t)UserEvent::SetConfig;

						size_t size = args[0].StringValue.length() + 1;
						event.user.data1 = new char[size];
						strcpy_s((char*)event.user.data1, size, args[0].StringValue.c_str());

						switch (var.GetFlags())
						{
						case HZ_CVAR_BOOL:
							event.user.data2 = new bool(var.GetBool());
							break;

						case HZ_CVAR_INT:
							event.user.data2 = new int(var.GetInt());
							break;

						case HZ_CVAR_UNSIGNED:
							event.user.data2 = new unsigned int(var.GetUnsigned());
							break;

						case HZ_CVAR_FLOAT:
							event.user.data2 = new float(var.GetFloat());
							break;

						default:
							LI_CORE_ERROR("Unknown CVAR type!");
							break;
						}

						SDL_PushEvent(&event);
						LI_TRACE("Pushed change event for console variable {}", args[0].StringValue);
					}
				}
				else
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
#endif