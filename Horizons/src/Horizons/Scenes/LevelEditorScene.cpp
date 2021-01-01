#include "pch.h"
#ifndef LI_DIST
#include "LevelEditorScene.h"

LevelEditorScene::LevelEditorScene()
{
}

LevelEditorScene::~LevelEditorScene()
{
	Li::Application::Get().PopLayer("LevelEditor");
}

void LevelEditorScene::OnShow()
{
	Li::Application& app = Li::Application::Get();
	app.PushLayer(Li::MakeUnique<LevelEditorLayer>());
	app.GetImGuiRenderer()->SetBlockEvents(false);
}

void LevelEditorScene::OnTransition()
{
}

#endif // !LI_DIST
