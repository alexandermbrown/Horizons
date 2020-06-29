#include "pch.h"
#include "SplashScreenScene.h"

#include "GameScene.h"

SplashScreenScene::SplashScreenScene()
	: m_SplashScreenLayer(nullptr)
{
}

void SplashScreenScene::TransitionIn()
{
	m_SplashScreenLayer = new SplashScreenLayer();
	li::Application::Get()->PushLayer(m_SplashScreenLayer);
	li::ResourceManager::LoadAsync("data/resources.lab");
}

void SplashScreenScene::TransitionOut()
{
	li::Ref<li::Window>& window = li::Application::Get()->GetWindow();
	window->SetBordered(true);
	window->SetSize(1280, 720);
	window->SetPosition(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	window->SetResizable(true);
	window->SetIcon("data/images/icon.png");
}

void SplashScreenScene::OnUpdate(float dt)
{
	if (!li::ResourceManager::DequeueAsset() && li::ResourceManager::IsLoaded())
	{
		li::ResourceManager::PrintInfo();

		// TODO: make config.
		li::Localization::SetLocale("en-us");

		li::Application* app = li::Application::Get();
		app->PopLayer(m_SplashScreenLayer);
		app->Transition(new GameScene());
	}
}
