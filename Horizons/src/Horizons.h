#pragma once

#include "Lithium.h"

#include "Horizons/Core/Core.h"
#include "Horizons/Core/ConfigStore.h"
#include "Horizons/Core/AppState.h"

#ifdef HZ_CONSOLE_ENABLED
#include "Horizons/Layers/ConsoleLayer.h"
#endif

#include "entt/entt.hpp"

class Horizons : public li::Application
{
public:
	Horizons();
	virtual ~Horizons();

	void Shutdown();

	inline ConfigStore& GetConfig() { return m_ConfigStore; }

#ifdef HZ_CONSOLE_ENABLED
	inline ConsoleLayer* GetConsole() { return m_Console; }
#endif

private:

	void LoadConfig();
	void SaveConfig();

	ConfigStore m_ConfigStore;

#ifdef HZ_CONSOLE_ENABLED
	ConsoleLayer* m_Console;
#endif
};