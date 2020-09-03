#include "pch.h"
#include "LevelEditorScene.h"

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
}
