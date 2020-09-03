#pragma once

#include "Horizons/Core/Core.h"
#include "Lithium.h"

#include "Horizons/Layers/SplashScreenLayer.h"
#include "Horizons/Layers/ConfigUpdateLayer.h"

class SplashScreenScene : public li::Scene
{
public:

	SplashScreenScene();
	virtual ~SplashScreenScene() = default;

	virtual void TransitionIn() override;
	virtual void TransitionOut() override;

	virtual void OnUpdate(float dt) override;

	virtual bool Finished() override { return true; }

private:

	SplashScreenLayer m_SplashScreenLayer;
	ConfigUpdateLayer m_ConfigCleanLayer;
};