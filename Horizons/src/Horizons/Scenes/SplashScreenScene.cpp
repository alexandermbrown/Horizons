#include "pch.h"
#include "SplashScreenScene.h"

#include "MainMenuScene.h"
#include "Horizons.h"

#include "Horizons/Core/AppState.h"
#include "Horizons/Terrain/TerrainManager.h"

SplashScreenScene::SplashScreenScene()
	: m_SplashScreenLayer(), m_ConfigCleanLayer()
{
}

void SplashScreenScene::TransitionIn()
{
	Horizons* app = li::Application::Get<Horizons>();

	app->GetConfig().Get("app_state").SetUnsigned((unsigned int)AppState::SplashScreen);

	app->PushLayer(&m_ConfigCleanLayer);
	app->PushLayer(&m_SplashScreenLayer);

#ifdef LI_DEBUG
	li::ResourceManager::BeginStaggeredLoad("data/resources.lab-debug");
#else
	li::ResourceManager::BeginStaggeredLoad("data/resources.lab");
#endif
}

void SplashScreenScene::TransitionOut()
{
	Horizons* app = li::Application::Get<Horizons>();
	app->PopLayer(&m_ConfigCleanLayer);
	app->PopLayer(&m_SplashScreenLayer);

	int width = app->GetConfig().Get("window_width").GetInt();
	int height = app->GetConfig().Get("window_height").GetInt();

	li::Window* window = app->GetWindow();
	window->SetBordered(true);
	window->SetSize(width, height);
	window->SetPosition(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	window->SetResizable(true);
	window->SetIcon("data/images/icon.png");
}

void SplashScreenScene::OnUpdate(float dt)
{
	li::Application::Get()->GetWindow()->GetContext()->Clear();

	if (!li::ResourceManager::UpdateStaggeredLoad())
	{
		li::ResourceManager::PrintInfo();

		// TODO: Make the renderer not dependant on the resource manager
		// Instead, change the below function to set the shaders used for instancing, fonts, etc.
		// In future, move the ResourceManager into Horizons and remove any references in Lithium.
		li::Renderer::InitPostResourceLoad();
		TerrainManager::Init();

		// TODO: Get locale from config.
		li::Localization::SetLocale("en-us");
		
		li::Application::Get()->Transition(new MainMenuScene());
	}
}
