#include "pch.h"
#include "Horizons.h"

#include "Horizons/Terrain/NoiseStore.h"
#include "Horizons/Scenes/SplashScreenScene.h"
#include "Horizons/ECS/RegisterSystems.h"

#ifndef LI_DIST
#include "Horizons/Console/CVarCommands.h"
#include "Horizons/Layers/ConsoleLayer.h"
#endif

#ifdef HZ_PLATFORM_WINDOWS
#include <shlobj.h>
#endif

Horizons::Horizons(Li::RendererAPI renderer_api)
	: Li::Application({ renderer_api, "Horizons", 768, 384, false, true, true})
{
#ifdef HZ_DEBUG
	Li::ResourceManager::Load("data/preload.lab-debug");
#else
	Li::ResourceManager::Load("data/preload.lab");
#endif
	Li::Localization::Init();
	Li::Renderer::Init();

	RegisterSystems(m_SystemRegistry);
	LoadConfig();

	m_ScriptContext.InitApp("data/scripts/Init.lua");

#ifdef HZ_CONSOLE_ENABLED
	PushOverlay(Li::MakeUnique<ConsoleLayer>());

	m_Console.AddCommand(CreateCVarSetCommand());
	m_Console.AddCommand(CreateCVarGetCommand());
#endif

	uint32_t game_to_app = SDL_RegisterEvents(1);
	uint32_t app_to_game = SDL_RegisterEvents(1);

	LI_ASSERT(game_to_app != ((uint32_t)-1), "Failed to create user event.");
	LI_ASSERT(app_to_game != ((uint32_t)-1), "Failed to create user event.");

	m_ConfigStore.Add(ConfigVar{ "event_game_to_app", game_to_app, HZ_CVAR_UNSIGNED, false });
	m_ConfigStore.Add(ConfigVar{ "event_app_to_game", app_to_game, HZ_CVAR_UNSIGNED, false });

	m_ConfigStore.Add(ConfigVar{ "app_state", 0, HZ_CVAR_UNSIGNED,  false });

	Transition(Li::MakeUnique<SplashScreenScene>(), true);
}

Horizons::~Horizons()
{
	ClearScene();
	ClearLayers();
	NoiseStore::Shutdown();
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

Li::Unique<Li::Application> Li::CreateApplication()
{
	// TODO: Add fallback to OpenGL on Windows.
#ifdef LI_PLATFORM_WINDOWS
	return Li::MakeUnique<Horizons>(Li::RendererAPI::D3D11);
#else
	return Li::MakeUnique<Horizons>(Li::RendererAPI::OpenGL);
#endif
}
