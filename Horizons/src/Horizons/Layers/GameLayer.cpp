#include "pch.h"
#include "GameLayer.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

#include "Horizons.h"
#include "Horizons/Core/TickThread.h"
#include <thread>

GameLayer::GameLayer()
	: Layer("GameLayer"),
	m_Camera((float)li::Application::Get()->GetWindow()->GetWidth() / (float)li::Application::Get()->GetWindow()->GetHeight(), 10.0f, true),
	m_EventQueue(256ULL)
#ifdef HZ_PHYSICS_DEBUG_DRAW
	, m_DebugDrawQueue(256ULL), m_DebugPhysicsRenderer(&m_DebugDrawQueue)
#endif
{
#ifdef HZ_PHYSICS_DEBUG_DRAW
	m_TickThread = std::thread(TickThreadEntryPointDebugDraw, &m_EventQueue, &m_DebugDrawQueue);
#else
	m_TickThread = std::thread(TickThreadEntryPoint, &m_EventQueue);
#endif

	m_Transform = glm::mat4(1.0f);

	li::Renderer::AddTextureAtlas(li::ResourceManager::Get<li::TextureAtlas>("atlas_test"));
}

void GameLayer::OnAttach()
{
	m_AudioSource = li::CreateRef<li::AudioSource>();
	m_AudioSource->SetAudio(li::ResourceManager::Get<li::Audio>("audio_wind"));
	m_AudioSource->Play();
}

void GameLayer::OnDetach()
{
	m_TickThread.join();
}

void GameLayer::OnUpdate(float dt)
{
	m_Camera.OnUpdate(dt);

	li::Renderer::BeginScene(&m_Camera.GetCamera());

	//li::Renderer::SubmitColored(m_QuadColor, glm::translate(m_Transform, glm::vec3(2.0f, 1.0f, 0.0f)));
	li::Renderer::SubmitTextured("test_small", glm::translate(m_Transform, glm::vec3(-2.0f, 1.0f, 0.0f)));
	//li::Renderer::SubmitLabel(m_Label, glm::scale(glm::mat4(1.0f), {0.01f, 0.01f, 1.0f}), glm::vec4(1.0f));

#ifdef HZ_PHYSICS_DEBUG_DRAW
	m_DebugPhysicsRenderer.Render(&m_Camera.GetCamera(), 0.45f);
#endif

	li::Renderer::EndScene();
}

void GameLayer::OnImGuiRender()
{
}

void GameLayer::OnEvent(SDL_Event* event)
{
	m_EventQueue.enqueue(*event);

	m_Camera.OnEvent(event);

	li::EventDispatcher dispatcher(event);
	dispatcher.Dispatch(SDL_KEYUP, [&](SDL_Event* e) {

		if (e->key.keysym.scancode == SDL_SCANCODE_F11) {
			if (li::Application::Get()->GetWindow()->GetFullscreen() == li::FullscreenType::Windowed) {
				li::Application::Get()->GetWindow()->SetFullscreen(li::FullscreenType::FullscreenWindowed);
			}
			else {
				li::Application::Get()->GetWindow()->SetFullscreen(li::FullscreenType::Windowed);
			}
		}
	});
}
