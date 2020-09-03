#pragma once

#include "Horizons/Layers/LevelEditorLayer.h"

#include "Lithium.h"

class LevelEditorScene : public li::Scene
{
public:
	LevelEditorScene();
	virtual ~LevelEditorScene();

	virtual void TransitionIn() override;
	virtual void TransitionOut() override;

	virtual void OnUpdate(float dt) override;

	virtual bool Finished() override { return true; }

private:
	LevelEditorLayer m_LevelEditorLayer;
};