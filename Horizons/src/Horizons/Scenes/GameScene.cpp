#include "pch.h"
#include "GameScene.h"

#include "Horizons.h"
#include "Horizons/Core/AppState.h"

#include "MainMenuScene.h"

GameScene::GameScene()
	: m_GameLayer(), m_HUDLayer()
{
}

void GameScene::TransitionIn()
{
	Horizons* app = li::Application::Get<Horizons>();
	app->GetConfig().Get("app_state").SetUnsigned((unsigned int)AppState::InGame);
	app->PushLayer(&m_GameLayer);
	app->PushLayer(&m_HUDLayer);
}

void GameScene::TransitionOut()
{
	Horizons* app = li::Application::Get<Horizons>();
	app->PopLayer(&m_GameLayer);
	app->PopLayer(&m_HUDLayer);
}

void GameScene::OnUpdate(float dt)
{
	if (m_GameLayer.ReturnToMainMenu())
	{
		li::Application::Get()->Transition(new MainMenuScene());
	}
}
