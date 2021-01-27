#include "pch.h"
#include "ConfigReceiveSystem.h"

#include "Horizons/Core/Game.h"
#include "Horizons/Core/UserEvents.h"

bool ConfigReceiveSystem::OnEvent(SDL_Event* event)
{
	ConfigStore& store = Game::Get()->GetConfig();

	if (event->type == store.Get<int>("event_app_to_game") && event->user.code == static_cast<int32_t>(UserEvent::SetConfig))
	{
		LI_ASSERT(event->user.data1, "Event must contain a string in data1.");
		LI_ASSERT(event->user.data2, "Event must contain data in data2.");

		std::string* name = static_cast<std::string*>(event->user.data1);

		auto it = store.find(*name);
		LI_ASSERT(it != store.end(), "Config variable not found.");

		if (std::holds_alternative<bool>(it->second))
			it->second = *static_cast<bool*>(event->user.data2);
		else if (std::holds_alternative<int>(it->second))
			it->second = *static_cast<int*>(event->user.data2);
		else if (std::holds_alternative<float>(it->second))
			it->second = *static_cast<float*>(event->user.data2);
		else if (std::holds_alternative<std::string>(it->second))
			it->second = *static_cast<std::string*>(event->user.data2);
		else {
			LI_ASSERT(false, "Unknown config variable type!");
		}
		delete event->user.data1;
		delete event->user.data2;

		return true;
	}
	return false;
}
