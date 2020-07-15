#include "pch.h"
#include "SplashScreenScene.h"

#include "GameScene.h"
#include "Horizons.h"

#include "Horizons/Core/AppState.h"

SplashScreenScene::SplashScreenScene()
	: m_SplashScreenLayer(nullptr), m_ConfigCleanLayer(nullptr)
{
}

void SplashScreenScene::TransitionIn()
{
	Horizons* app = li::Application::Get()->Get<Horizons>();

	app->GetConfig().Get("app_state").SetUnsigned((unsigned int)AppState::SplashScreen);

	m_ConfigCleanLayer = new ConfigUpdateLayer();
	app->PushLayer(m_ConfigCleanLayer);

	m_SplashScreenLayer = new SplashScreenLayer();
	app->PushLayer(m_SplashScreenLayer);

	li::ResourceManager::LoadAsync("data/resources.lab");
}

void SplashScreenScene::TransitionOut()
{
	Horizons* app = li::Application::Get<Horizons>();
	app->PopLayer(m_SplashScreenLayer);

	int width = app->GetConfig().Get("window_width").GetInt();
	int height = app->GetConfig().Get("window_height").GetInt();

	li::Ref<li::Window>& window = app->GetWindow();
	window->SetBordered(true);
	window->SetSize(width, height);
	window->SetPosition(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	window->SetResizable(true);
	window->SetIcon("data/images/icon.png");

	m_ConfigCleanLayer->FinishSplashScreen();
}

void SplashScreenScene::OnUpdate(float dt)
{
	li::RendererAPI::Clear();

	if (!li::ResourceManager::DequeueAsset() && li::ResourceManager::IsLoaded())
	{
		li::ResourceManager::PrintInfo();

		// TODO: make config.
		li::Localization::SetLocale("en-us");

		
		
		li::Application::Get()->Transition(new GameScene());
	}
}
