#pragma once

#include "Lithium.h"

class LevelEditorScene : public li::Scene
{
public:
	virtual ~LevelEditorScene() = default;

	virtual void TransitionIn() override;
	virtual void TransitionOut() override;

	virtual void OnUpdate(float dt) override;

	virtual bool Finished() override { return true; }
};