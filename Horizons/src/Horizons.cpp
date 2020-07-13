#include "pch.h"
#include "Horizons.h"

#include "Horizons/Scenes/SplashScreenScene.h"

#ifdef HZ_CONSOLE_ENABLED
#include "Horizons/Commands/CVarCommands.h"
#endif

#include "Lithium/Core/EntryPoint.h"

#ifdef HZ_PLATFORM_WINDOWS
#include "SDL_syswm.h"
#include <shlobj.h>
#endif

Horizons::Horizons()
	: li::Application({ "Horizons", 768, 384, false, true, true })
{
	LoadConfig();

#ifdef HZ_CONSOLE_ENABLED
	m_Console = new ConsoleLayer();
	PushOverlay(m_Console);

	m_Console->AddCommand(CreateCVarSetCommand());
	m_Console->AddCommand(CreateCVarGetCommand());
#endif


	uint32_t game_to_app = SDL_RegisterEvents(1);
	uint32_t app_to_game = SDL_RegisterEvents(1);

	LI_ASSERT(game_to_app != ((uint32_t)-1), "Failed to create user event.");
	LI_ASSERT(app_to_game != ((uint32_t)-1), "Failed to create user event.");

	m_ConfigStore.Add(ConfigVar("event_game_to_app", HZ_CVAR_UNSIGNED, game_to_app, false));
	m_ConfigStore.Add(ConfigVar("event_app_to_game", HZ_CVAR_UNSIGNED, app_to_game, false));

	Transition(new SplashScreenScene());
}

Horizons::~Horizons()
{
	SaveConfig();
}

void Horizons::LoadConfig()
{
	m_ConfigStore.LoadTemplate("data/config/config_default.ini");

#ifdef HZ_PLATFORM_WINDOWS
	// Get the location of %APPDATA% in Windows.
	wchar_t* path;
	SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &path);

	std::wstring path_str = path;
	path_str += L"\\Horizons\\config\\config.ini";

	m_ConfigStore.LoadConfigFile(path_str.c_str());
#else
	// TODO: Support other platforms.
#error Other platforms not supported.
#endif
}

void Horizons::SaveConfig()
{
#ifdef HZ_PLATFORM_WINDOWS
	// Get the location of %APPDATA% in Windows.
	wchar_t* path;
	SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &path);

	std::wstring path_str = path;
	path_str += L"\\Horizons\\config\\config.ini";

	m_ConfigStore.SaveConfigFile(path_str.c_str());
#endif
}

li::Application* li::CreateApplication()
{
	return new Horizons();
}
