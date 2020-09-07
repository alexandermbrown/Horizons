#pragma once

#include "Horizons/Core/Core.h"
#include "Lithium.h"

class SplashScreenLayer : public li::Layer
{
public:
	SplashScreenLayer();
	virtual ~SplashScreenLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(float dt) override;
	virtual void OnEvent(SDL_Event* event) override;
#ifndef LI_DIST
	virtual void OnImGuiRender() override;
#endif

private:

	void CalculateTransform();

	li::Ref<li::Texture2D> m_Texture;
	glm::mat4 m_Transform;
};