#include "pch.h"
#include "GameScene.h"

#include "Horizons/Layers/GameLayer.h"

void GameScene::TransitionIn()
{
	li::Application::Get()->PushLayer(new GameLayer());
}

void GameScene::TransitionOut()
{
}

void GameScene::OnUpdate(float dt)
{
}
