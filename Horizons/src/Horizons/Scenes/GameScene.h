#pragma once

#include "Horizons/Core/Core.h"
#include "Lithium.h"

class GameScene : public li::Scene
{
public:
	virtual ~GameScene() = default;

	virtual void TransitionIn() override;
	virtual void TransitionOut() override;

	virtual void OnUpdate(float dt) override;

	virtual bool Finished() override { return true; }
};