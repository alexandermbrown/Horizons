#include "pch.h"
#include "GameScene.h"

#include "Horizons.h"
#include "Horizons/Core/AppState.h"

#include "Horizons/Layers/GameLayer.h"
#include "Horizons/Layers/HUDLayer.h"

void GameScene::TransitionIn()
{
	li::Application::Get()->Get<Horizons>()->GetConfig().Get("app_state").SetUnsigned((unsigned int)AppState::InGame);
	li::Application::Get()->PushLayer(new GameLayer());
	li::Application::Get()->PushLayer(new HUDLayer());
}

void GameScene::TransitionOut()
{
}

void GameScene::OnUpdate(float dt)
{
	
}
