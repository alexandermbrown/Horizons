#pragma once
#ifndef LI_DIST
#include "Horizons/Layers/LevelEditorLayer.h"

#include "Lithium.h"

class LevelEditorScene : public Li::Scene
{
public:
	LevelEditorScene();
	virtual ~LevelEditorScene();

	virtual void OnShow() override;
	virtual void OnTransition() override;
};
#endif