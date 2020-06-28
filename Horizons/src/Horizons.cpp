#include "pch.h"
#include "Horizons.h"

#include "Horizons/Layers/GameLayer.h"
#include "Horizons/Layers/SplashScreenLayer.h"

#ifdef HZ_CONSOLE_ENABLED
#include "Horizons/Commands/CVarCommands.h"
#endif

#include "Lithium/Core/EntryPoint.h"

Horizons::Horizons()
	: li::Application({ "Horizons", 768, 384, false, true, true })
{

#ifdef HZ_CONSOLE_ENABLED
	m_Console = new ConsoleLayer();
	PushOverlay(m_Console);

	m_ConfigStore.Add(ConfigVar("jeff", 1.0f, HZ_CVAR_FLOAT, "Test cvar!"));
	m_Console->AddCommand(CreateCVarSetCommand());
	m_Console->AddCommand(CreateCVarGetCommand());
#endif

	PushLayer(new SplashScreenLayer());
}

li::Application* li::CreateApplication()
{
	return new Horizons();
}
