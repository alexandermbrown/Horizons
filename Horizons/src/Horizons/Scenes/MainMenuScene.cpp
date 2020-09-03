#include "pch.h"
#include "MainMenuScene.h"

#include "Horizons.h"
#include "Horizons/Core/AppState.h"

MainMenuScene::MainMenuScene()
	: m_MainMenuLayer(), m_Diagnostics()
{
}

MainMenuScene::~MainMenuScene()
{
	li::Application* app = li::Application::Get();
	app->PopLayer(&m_MainMenuLayer);
	app->PopOverlay(&m_Diagnostics);
}

void MainMenuScene::TransitionIn()
{
	Horizons* app = li::Application::Get<Horizons>();
	app->GetConfig().Get("app_state").SetUnsigned((unsigned int)AppState::MainMenu);
	app->PushLayer(&m_MainMenuLayer);
	app->PushOverlay(&m_Diagnostics);
}

void MainMenuScene::TransitionOut()
{
}

void MainMenuScene::OnUpdate(float dt)
{
	li::Scene* scene = m_MainMenuLayer.GetTransitionScene();
	if (scene)
	{
		li::Application::Get()->Transition(scene);
	}
}
