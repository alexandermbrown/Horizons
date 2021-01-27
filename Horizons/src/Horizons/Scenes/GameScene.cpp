#include "pch.h"
#include "GameScene.h"

#include "Horizons.h"
#include "Horizons/Core/AppState.h"

#include "Horizons/Layers/GameLayer.h"

void GameScene::OnShow()
{
	Horizons& app = Li::Application::Get<Horizons>();
	app.GetConfig().Set<int>("app_state", static_cast<int>(AppState::InGame));
	app.PushLayer(Li::MakeUnique<GameLayer>());
#ifndef LI_DIST
	app.GetImGuiRenderer()->SetBlockEvents(true);
#endif
}

void GameScene::OnTransition()
{
	Horizons& app = Li::Application::Get<Horizons>();
	app.PopLayer("GameLayer");
}
