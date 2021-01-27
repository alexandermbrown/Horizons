#include "pch.h"
#include "SplashScreenScene.h"

#include "Horizons/Layers/SplashScreenLayer.h"
#include "Horizons/Layers/DiagnosticsLayer.h"

#include "Horizons.h"

#include "Horizons/Core/AppState.h"
#include "Horizons/Scripting/TerrainData.h"

void SplashScreenScene::OnShow()
{
	Horizons& app = Li::Application::Get<Horizons>();

	app.GetConfig().Set<int>("app_state", static_cast<int>(AppState::SplashScreen));

	app.PushLayer(Li::MakeUnique<SplashScreenLayer>());
#ifndef HZ_DIST
	app.PushOverlay(Li::MakeUnique<DiagnosticsLayer>());
#endif

#ifdef HZ_DEBUG
	Li::ResourceManager::BeginStaggeredLoad("data/resources.lab-debug");
#else
	Li::ResourceManager::BeginStaggeredLoad("data/resources.lab");
#endif
}

void SplashScreenScene::OnTransition()
{
	Horizons& app = Li::Application::Get<Horizons>();
	app.PopLayer("SplashScreen");

	int width = app.GetConfig().Get<int>("window_width");
	int height = app.GetConfig().Get<int>("window_height");

	Li::Window& window = app.GetWindow();
	window.SetBordered(true);
	window.SetSize(width, height);
	window.SetPosition(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	window.SetResizable(true);
	window.SetIcon("data/images/icon.png");
}
