#include "pch.h"
#ifndef LI_DIST
#include "LevelEditorScene.h"

#include "MainMenuScene.h"

LevelEditorScene::LevelEditorScene()
{
}

LevelEditorScene::~LevelEditorScene()
{
	li::Application* app = li::Application::Get();
	app->PopLayer(&m_LevelEditorLayer);
}

void LevelEditorScene::TransitionIn()
{
	li::Application* app = li::Application::Get();
	app->PushLayer(&m_LevelEditorLayer);
}

void LevelEditorScene::TransitionOut()
{
	
}

void LevelEditorScene::OnUpdate(float dt)
{
	if (m_LevelEditorLayer.ReturnToMainMenu())
	{
		li::Application::Get()->Transition(new MainMenuScene());
	}
}
#endif // !LI_DIST
