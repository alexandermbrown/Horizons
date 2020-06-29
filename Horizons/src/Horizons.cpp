#include "pch.h"
#include "Horizons.h"

#include "Horizons/Scenes/SplashScreenScene.h"

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

	m_ConfigStore.Add(ConfigVar("jeff", 32, HZ_CVAR_INT, "Test cvar!"));
	m_Console->AddCommand(CreateCVarSetCommand());
	m_Console->AddCommand(CreateCVarGetCommand());
#endif

	Transition(new SplashScreenScene());
}

li::Application* li::CreateApplication()
{
	return new Horizons();
}
