#pragma once

#include "Lithium.h"

#include "Horizons/Core/Core.h"
#include "Horizons/Core/ConfigVar.h"

#ifdef HZ_CONSOLE_ENABLED
#include "Horizons/Layers/ConsoleLayer.h"
#endif

class Horizons : public li::Application
{
public:
	Horizons();
	virtual ~Horizons() = default;

	inline ConfigStore& GetConfig() { return m_ConfigStore; }

#ifdef HZ_CONSOLE_ENABLED
	inline ConsoleLayer* GetConsole() { return m_Console; }
#endif

private:

	ConfigStore m_ConfigStore;

#ifdef HZ_CONSOLE_ENABLED
	ConsoleLayer* m_Console;
#endif
};