#include "pch.h"
#include "GameScene.h"

#include "Horizons.h"
#include "Horizons/Core/AppState.h"

void GameScene::TransitionIn()
{
	Horizons* app = li::Application::Get<Horizons>();
	app->GetConfig().Get("app_state").SetUnsigned((unsigned int)AppState::InGame);
	app->PushLayer(new GameLayer());
	app->PushLayer(new HUDLayer());
}

void GameScene::TransitionOut()
{
	Horizons* app = li::Application::Get<Horizons>();
	app->PopLayer(m_GameLayer);
	app->PopLayer(m_HUDLayer);
}

void GameScene::OnUpdate(float dt)
{
	
}
