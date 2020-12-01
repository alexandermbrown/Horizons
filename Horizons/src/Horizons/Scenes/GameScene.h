#pragma once

#include "Horizons/Core/Core.h"
#include "Lithium.h"

#include "Horizons/Layers/GameLayer.h"
#include "Horizons/Layers/HUDLayer.h"
#include "Horizons/Layers/DiagnosticsLayer.h"

class GameScene : public li::Scene
{
public:
	GameScene();
	virtual ~GameScene() = default;

	virtual void TransitionIn() override;
	virtual void TransitionOut() override;

	virtual void OnUpdate(li::duration::us dt) override;

	virtual bool Finished() override { return true; }

private:
	GameLayer m_GameLayer;
	HUDLayer m_HUDLayer;
#ifndef LI_DIST
	DiagnosticsLayer m_Diagnostics;
#endif
};