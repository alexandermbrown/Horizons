#include "pch.h"
#include "GameLayer.h"

#include "Horizons.h"
#include "Horizons/Scripting/ScriptScene.h"
#include "Horizons/Gameplay/TransformComponent.h"
#include "Horizons/Rendering/RenderingSystem.h"
#include "Horizons/Rendering/RenderingComponents.h"
#include "Horizons/Gameplay/TransformUpdateSystem.h"
#include "Horizons/Gameplay/CameraControllerSystem.h"

#include "Horizons/Gameplay/Player/PlayerComponents.h"

#include "glm/gtc/type_ptr.hpp"

GameLayer::GameLayer()
	: Layer("GameLayer"), m_TerrainStore(11), m_TerrainRenderer(&m_TerrainStore, 3), m_Registry()
{
	m_TickThread.Begin(m_Registry);

	CameraControllerSystem::Init(m_Registry);

	m_TerrainRenderer.LoadTerrain("data/worlds/test.terrain", { 0, 0 });

	m_AudioSource = Li::MakeRef<Li::AudioSource>();
	m_AudioSource->SetAudio(Li::ResourceManager::GetAudioBuffer("audio_wind"));
	m_AudioSource->Play();
}

GameLayer::~GameLayer()
{
	CameraControllerSystem::Shutdown(m_Registry);
	m_TerrainRenderer.UnloadTerrain();
	m_TickThread.Finish(m_Registry);
}

void GameLayer::OnUpdate(Li::Duration::us dt)
{
	m_TickThread.UpdateSync(m_Registry, dt);

	auto player_view = m_Registry.view<cp::sync_transform, cp::player>();
	for (entt::entity player : player_view)
	{
		cp::sync_transform& player_transform = player_view.get<cp::sync_transform>(player);

		m_TerrainRenderer.UpdateCenter({
			(int)std::floor(player_transform.position.x / TerrainRenderer::MetersPerChunk),
			(int)std::floor(player_transform.position.y / TerrainRenderer::MetersPerChunk)
		});
		break;
	}

	CameraControllerSystem::Update(m_Registry, dt);

	TransformUpdateSystem::Update(m_Registry);

	cp::camera& camera = m_Registry.ctx<cp::camera>();

	m_TerrainRenderer.RenderFramebuffer();
	
	Li::Renderer::BeginScene(camera.camera);

	m_TerrainRenderer.SubmitQuad();
	RenderingSystem::Render(m_Registry);

#ifdef HZ_PHYSICS_DEBUG_DRAW
	m_DebugPhysicsRenderer.SubmitLines(m_TickThread.GetDebugDrawQueue());
#endif
	Li::Renderer::EndScene();
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
			if (Li::Application::Get().GetWindow().GetFullscreen() == Li::FullscreenType::Windowed) {
				Li::Application::Get().GetWindow().SetFullscreen(Li::FullscreenType::FullscreenWindowed);
			}
			else {
				Li::Application::Get().GetWindow().SetFullscreen(Li::FullscreenType::Windowed);
			}
			break;
		case SDL_SCANCODE_ESCAPE:
			Li::Application::Get().Transition(Li::MakeUnique<ScriptScene>("MainMenuScene"), true);
			break;
		}
	}
}

#ifndef LI_DIST
void GameLayer::OnImGuiRender()
{
}
#endif
