#include "pch.h"
#include "Horizons.h"

#include "Horizons/Layers/GameLayer.h"

#ifdef HZ_CONSOLE_ENABLED
#include "Horizons/Commands/CVarCommands.h"
#endif

#include "Lithium/Core/EntryPoint.h"

Horizons::Horizons()
	: li::Application()
{
	li::ResourceManager::Init("data/resources.lab");
	li::Localization::SetLocale("en-us");
	PushLayer(new GameLayer());

#ifdef HZ_CONSOLE_ENABLED
	m_Console = new ConsoleLayer();
	PushOverlay(m_Console);

	m_ConfigStore.Add(ConfigVar("jeff", 1.0f, HZ_CVAR_FLOAT, "Test cvar!"));
	m_Console->AddCommand(CreateCVarSetCommand());
	m_Console->AddCommand(CreateCVarGetCommand());
#endif
}

li::Application* li::CreateApplication()
{
	return new Horizons();
}
