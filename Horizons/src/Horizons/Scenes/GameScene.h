#pragma once

#include "Horizons/Core/Core.h"
#include "Lithium.h"

class GameScene : public Li::Scene
{
public:
	virtual ~GameScene() = default;

	virtual void OnShow() override;
	virtual void OnTransition() override;
};
