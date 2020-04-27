#pragma once

#include "Lithium.h"
#include <glm/glm.hpp>

class GameLayer : public li::Layer
{
public:
	GameLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(float dt) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(li::Event& event) override;

private:
	li::Ref<li::Texture2D> m_TestTexture;
	
	li::OrthographicCameraController m_Camera;
	glm::mat4 m_Transform;
	glm::vec4 m_QuadColor;

	li::Ref<li::Label> m_Label;
};