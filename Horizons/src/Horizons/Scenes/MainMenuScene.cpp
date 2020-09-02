#include "pch.h"
#include "MainMenuScene.h"

#include "Horizons/Layers/DiagnosticsLayer.h"

void MainMenuScene::TransitionIn()
{
	li::Application* app = li::Application::Get();

	m_MainMenuLayer = new MainMenuLayer();
	app->PushLayer(m_MainMenuLayer);
	app->PushOverlay(new DiagnosticsLayer());
}

void MainMenuScene::TransitionOut()
{
	li::Application* app = li::Application::Get();
	app->PopLayer(m_MainMenuLayer);
}

void MainMenuScene::OnUpdate(float dt)
{
}
