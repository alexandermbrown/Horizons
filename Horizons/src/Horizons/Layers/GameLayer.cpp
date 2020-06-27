#include "pch.h"
#include "GameLayer.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

#include "Horizons/Core/TickThread.h"
#include <thread>

GameLayer::GameLayer()
	: Layer("GameLayer"),
	m_Camera((float)li::Application::Get()->GetWindow()->GetWidth() / (float)li::Application::Get()->GetWindow()->GetHeight(), 10.0f, true),
	m_QuadColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)), m_EventQueue(256ULL)
#ifdef HZ_PHYSICS_DEBUG_DRAW
	, m_DebugDrawQueue(256ULL), m_DebugPhysicsRenderer(&m_DebugDrawQueue)
#endif
{
	m_Label = li::CreateRef<li::Label>(li::Localization::Get("BUTTON_PLAY"), 30.0f,
		li::ResourceManager::Get<li::Font>("RobotoMono-Italic"), glm::vec4(1.0f), 11);

	m_Transform = glm::mat4(1.0f);

	li::Renderer::AddTextureAtlas(li::ResourceManager::Get<li::TextureAtlas>("atlas_test"));

#ifdef HZ_PHYSICS_DEBUG_DRAW
	m_TickThread = std::thread(TickThreadEntryPointDebugDraw, &m_EventQueue, &m_DebugDrawQueue);
#else
	m_TickThread = std::thread(TickThreadEntryPoint, &m_EventQueue);
#endif
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

	li::Renderer::SubmitColored(m_QuadColor, glm::translate(m_Transform, glm::vec3(2.0f, 1.0f, 0.0f)));
	li::Renderer::SubmitColoredTexture("test_small", m_QuadColor, glm::translate(m_Transform, glm::vec3(-2.0f, 1.0f, 0.0f)));
	li::Renderer::SubmitLabel(m_Label, glm::scale(glm::mat4(1.0f), {0.01f, 0.01f, 1.0f}), glm::vec4(1.0f));


	glm::mat4 uiTransform = glm::translate(glm::mat4(1.0f), { 20.0f, 20.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { 200.0f, 500.0f, 0.9f });

	//li::Renderer::UISubmitColored({ 0.3f, 0.8f, 0.8f, 1.0f }, uiTransform);
	li::Renderer::UISubmitLabel(m_Label, glm::translate(glm::mat4(1.0f), { 100.0f, 100.0f, 1.0f }), glm::vec4(1.0f));

	li::Renderer::EndScene();

#ifdef HZ_PHYSICS_DEBUG_DRAW
	m_DebugPhysicsRenderer.Render(&m_Camera.GetCamera(), 0.9f);
#endif
}

void GameLayer::OnImGuiRender()
{
	ImGui::Begin("Horizons");
	ImGui::ColorEdit4("Quad Color", glm::value_ptr(m_QuadColor));
	ImGui::End();
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
		else if (e->key.keysym.scancode == SDL_SCANCODE_L) {

			if (li::Localization::GetCode() == "en-us") {
				li::Localization::SetLocale("test");
			}
			else {
				li::Localization::SetLocale("en-us");
			}

			m_Label->SetText(li::Localization::Get("BUTTON_PLAY"));
		}
	});
}
