#include "pch.h"
#include "GameScene.h"

#include "Horizons/Layers/GameLayer.h"
#include "Horizons/Layers/HUDLayer.h"
#include "Horizons/Layers/DiagnosticsLayer.h"

void GameScene::TransitionIn()
{
	li::Application::Get()->PushLayer(new GameLayer());
	li::Application::Get()->PushLayer(new HUDLayer());
	li::Application::Get()->PushOverlay(new DiagnosticsLayer());
}

void GameScene::TransitionOut()
{
}

void GameScene::OnUpdate(float dt)
{
	li::RendererAPI::Clear();
}
