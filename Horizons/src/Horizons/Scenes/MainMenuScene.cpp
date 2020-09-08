#include "pch.h"
#include "MainMenuScene.h"

#include "Horizons.h"
#include "Horizons/Core/AppState.h"

MainMenuScene::MainMenuScene()
	: m_MainMenuLayer()
#ifndef LI_DIST
	, m_Diagnostics()
#endif
{
}

MainMenuScene::~MainMenuScene()
{
	li::Application* app = li::Application::Get();
	app->PopLayer(&m_MainMenuLayer);
#ifndef LI_DIST
	app->PopOverlay(&m_Diagnostics);
#endif
}

void MainMenuScene::TransitionIn()
{
	Horizons* app = li::Application::Get<Horizons>();
	app->GetConfig().Get("app_state").SetUnsigned((unsigned int)AppState::MainMenu);
	app->PushLayer(&m_MainMenuLayer);
#ifndef LI_DIST
	app->PushOverlay(&m_Diagnostics);
	app->GetImGuiRenderer()->SetBlockEvents(true);
#endif
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
