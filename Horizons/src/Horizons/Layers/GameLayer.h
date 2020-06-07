#pragma once

#include "Horizons/Core/Core.h"

#ifdef HZ_PHYSICS_DEBUG_DRAW
#include "Horizons/Rendering/DebugPhysicsDrawShared.h"
#include "Horizons/Rendering/DebugPhysicsRenderer.h"
#endif

#include "Horizons/Gameplay/Player/OrthographicCameraController.h"

#include "Lithium.h"
#include "glm/glm.hpp"
#include "readerwriterqueue/readerwriterqueue.h"

class GameLayer : public li::Layer
{
public:
	GameLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(float dt) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(SDL_Event* event) override;

private:
	li::Ref<li::Texture2D> m_TestTexture;
	
	OrthographicCameraController m_Camera;
	glm::mat4 m_Transform;
	glm::vec4 m_QuadColor;

	li::Ref<li::Label> m_Label;

	li::Ref<li::AudioSource> m_AudioSource;

	std::thread m_TickThread;

	moodycamel::ReaderWriterQueue<SDL_Event> m_EventQueue;

#ifdef HZ_PHYSICS_DEBUG_DRAW
	DebugDrawCommandQueue m_DebugDrawQueue;
	DebugPhysicsRenderer m_DebugPhysicsRenderer;
#endif
};