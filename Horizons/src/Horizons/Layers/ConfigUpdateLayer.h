#pragma once

#include "Horizons/Core/Core.h"

#include "Lithium.h"
#include "glm/glm.hpp"
#include "readerwriterqueue/readerwriterqueue.h"

class ConfigUpdateLayer : public li::Layer
{
public:
	ConfigUpdateLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(float dt) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(SDL_Event* event) override;

	inline void FinishSplashScreen() { m_OnSplashScreen = false; }

private:

	void Clean();

	bool m_OnSplashScreen = true;
};