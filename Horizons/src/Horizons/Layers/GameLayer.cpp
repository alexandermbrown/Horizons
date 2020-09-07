#include "pch.h"
#include "GameLayer.h"

#include "Horizons.h"
#include "Horizons/Gameplay/Components.h"
#include "Horizons/Terrain/TerrainManager.h"
#include "Horizons/Rendering/RenderingSystem.h"
#include "Horizons/Rendering/RenderingComponents.h"
#include "Horizons/Gameplay/TransformUpdateSystem.h"
#include "Horizons/Gameplay/CameraControllerSystem.h"

#include "Horizons/Gameplay/Player/PlayerComponents.h"
#include "Horizons/Gameplay/Components.h"

#include "Horizons/Terrain/SimplexNoise.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

GameLayer::GameLayer()
	: Layer("GameLayer"), m_Registry(), m_ReturnToMainMenu(false)
{
	m_TickThread.Begin(m_Registry);

	li::Renderer::AddTextureAtlas(li::ResourceManager::Get<li::TextureAtlas>("atlas_test"));

	CameraControllerSystem::Init(m_Registry);

	TerrainManager::LoadWorld("data/worlds/test.terrain", { 0, 0 });

	m_AudioSource = li::CreateRef<li::AudioSource>();
	m_AudioSource->SetAudio(li::ResourceManager::Get<li::Audio>("audio_wind"));
	m_AudioSource->Play();
}

GameLayer::~GameLayer()
{
	CameraControllerSystem::Shutdown(m_Registry);
	TerrainManager::UnloadWorld();
	m_TickThread.Finish(m_Registry);
}

void GameLayer::OnAttach()
{
	
}

void GameLayer::OnDetach()
{
	
}

void GameLayer::OnUpdate(float dt)
{
	m_TickThread.UpdateSync(m_Registry, dt);

	auto player_view = m_Registry.view<cp::sync_transform, cp::player>();
	for (entt::entity player : player_view)
	{
		cp::sync_transform& player_transform = player_view.get<cp::sync_transform>(player);

		TerrainManager::UpdateCenter({
			(int)std::floor(player_transform.position.x / TerrainManager::MetersPerChunk),
			(int)std::floor(player_transform.position.y / TerrainManager::MetersPerChunk)
		});
		break;
	}

	CameraControllerSystem::Update(m_Registry, dt);

	TransformUpdateSystem::Update(m_Registry);

	cp::camera& camera = m_Registry.ctx<cp::camera>();

	TerrainManager::RenderFramebuffer();

	li::Application::Get()->GetWindow()->GetContext()->BindDefaultRenderTarget();
	li::Application::Get()->GetWindow()->GetContext()->Clear();
	li::Renderer::BeginScene(camera.camera);

	TerrainManager::SubmitQuad();
	RenderingSystem::Render(m_Registry);

	li::Renderer::EndScene();

#ifdef HZ_PHYSICS_DEBUG_DRAW
	li::Renderer::BeginScene(camera.camera);
	m_DebugPhysicsRenderer.Render(m_TickThread.GetDebugDrawQueue());
	li::Renderer::EndScene();
#endif
}

void GameLayer::OnEvent(SDL_Event* event)
{
	m_TickThread.OnEvent(event);
	CameraControllerSystem::OnEvent(m_Registry, event);

	if (event->type == SDL_KEYUP)
	{
		switch (event->key.keysym.scancode)
		{
		case SDL_SCANCODE_F11:
			if (li::Application::Get()->GetWindow()->GetFullscreen() == li::FullscreenType::Windowed) {
				li::Application::Get()->GetWindow()->SetFullscreen(li::FullscreenType::FullscreenWindowed);
			}
			else {
				li::Application::Get()->GetWindow()->SetFullscreen(li::FullscreenType::Windowed);
			}
			break;
		case SDL_SCANCODE_ESCAPE:
			m_ReturnToMainMenu = true;
			break;
		}
	}
}

#ifndef LI_DIST
void GameLayer::OnImGuiRender()
{
}
#endif