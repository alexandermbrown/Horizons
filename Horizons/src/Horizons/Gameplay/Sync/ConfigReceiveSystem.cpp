#include "pch.h"
#include "ConfigReceiveSystem.h"

#include "Horizons/Core/Game.h"
#include "Horizons/Core/UserEvents.h"

bool ConfigReceiveSystem::OnEvent(SDL_Event* event)
{
	ConfigStore& store = Game::Get()->GetConfig();

	if (event->type == store.Get("event_app_to_game").GetUnsigned() && event->user.code == (int32_t)UserEvent::SetConfig)
	{
		LI_ASSERT(event->user.data1, "Event must contain a string in data1.");
		LI_ASSERT(event->user.data2, "Event must contain data in data2.");

		ConfigVar& var = store.Get(static_cast<const char*>(event->user.data1));

		LI_TRACE("Received new value for console variable {}", static_cast<const char*>(event->user.data1));

		switch (var.GetFlags())
		{
		case HZ_CVAR_BOOL:
			var.SetBool(*static_cast<bool*>(event->user.data2));
			break;

		case HZ_CVAR_INT:
			var.SetInt(*static_cast<int*>(event->user.data2));
			break;

		case HZ_CVAR_UNSIGNED:
			var.SetUnsigned(*static_cast<unsigned int*>(event->user.data2));
			break;

		case HZ_CVAR_FLOAT:
			var.SetFloat(*static_cast<float*>(event->user.data2));
			break;

		default:
			LI_ERROR("Unknown CVAR type!");
			break;
		}

		delete event->user.data1;
		delete event->user.data2;

		return true;
	}
	return false;
}
