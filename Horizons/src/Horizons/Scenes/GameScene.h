#pragma once

#include "Horizons/Core/Core.h"
#include "Lithium.h"

#include "Horizons/Layers/GameLayer.h"
#include "Horizons/Layers/HUDLayer.h"

class GameScene : public li::Scene
{
public:
	virtual ~GameScene() = default;

	virtual void TransitionIn() override;
	virtual void TransitionOut() override;

	virtual void OnUpdate(float dt) override;

	virtual bool Finished() override { return true; }

private:
	GameLayer* m_GameLayer;
	HUDLayer* m_HUDLayer;
};