#pragma once

#include "Lithium.h"

#include "Horizons/Layers/MainMenuLayer.h"

class MainMenuScene : public li::Scene
{
public:
	virtual ~MainMenuScene() = default;

	virtual void TransitionIn() override;
	virtual void TransitionOut() override;

	virtual void OnUpdate(float dt) override;

	virtual bool Finished() override { return true; }

private:
	MainMenuLayer* m_MainMenuLayer;
};