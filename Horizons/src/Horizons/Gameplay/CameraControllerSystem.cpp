#include "pch.h"
#include "CameraControllerSystem.h"

#include "Horizons/Rendering/RenderingComponents.h"
#include "Horizons/Gameplay/Player/PlayerComponents.h"
#include "Horizons/Gameplay/Components.h"

#include "Lithium.h"

void CameraControllerSystem::Init(entt::registry& registry)
{
	cp::camera& camera = registry.set<cp::camera>();

	camera.current_zoom = 10.0f;
	camera.target_zoom = 10.0f;
	camera.aspect_ratio = (float)li::Application::Get()->GetWindow()->GetWidth()
		/ (float)li::Application::Get()->GetWindow()->GetHeight();

	camera.camera = new li::OrthographicCamera(-camera.aspect_ratio * camera.current_zoom,
		camera.aspect_ratio * camera.current_zoom, -camera.current_zoom, camera.current_zoom);
}

void CameraControllerSystem::Shutdown(entt::registry& registry)
{
	cp::camera& camera = registry.ctx<cp::camera>();
	LI_ASSERT(camera.camera, "Camera should not be nullptr.");
	delete camera.camera;
}

void CameraControllerSystem::Update(entt::registry& registry, float dt)
{
	cp::camera& camera = registry.ctx<cp::camera>();

	auto player_view = registry.view<cp::transform, cp::player>();
	for (entt::entity player : player_view)
	{
		cp::transform& player_transform = player_view.get<cp::transform>(player);
		if (camera.camera->GetPosition() != player_transform.position)
			camera.camera->SetPosition(player_transform.position);

		break;
	}
	
	if (!camera.finished_zoom)
	{
		float delta = camera.current_zoom - camera.target_zoom;
		if (dt < 0.99f)
			camera.current_zoom -= delta * dt * 10.0f;
		else
			camera.current_zoom = camera.target_zoom;

		camera.camera->SetProjection(-camera.aspect_ratio * camera.current_zoom,
			camera.aspect_ratio * camera.current_zoom, -camera.current_zoom, camera.current_zoom);

		if (camera.current_zoom == camera.target_zoom)
			camera.finished_zoom = true;
	}
}

void CameraControllerSystem::OnEvent(entt::registry& registry, SDL_Event* event)
{
	if (event->type == SDL_MOUSEWHEEL)
	{
		cp::camera& camera = registry.ctx<cp::camera>();

		camera.target_zoom -= event->wheel.y * std::sqrtf(camera.target_zoom) / 2.0f;

		if (camera.target_zoom < camera.min_zoom)
			camera.target_zoom = camera.min_zoom;

		else if (camera.target_zoom > camera.max_zoom)
			camera.target_zoom = camera.max_zoom;

		camera.finished_zoom = false;
	}
	else if (event->type == SDL_WINDOWEVENT)
	{
		if (event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
		{
			cp::camera& camera = registry.ctx<cp::camera>();

			camera.aspect_ratio = (float)li::Application::Get()->GetWindow()->GetWidth()
				/ (float)li::Application::Get()->GetWindow()->GetHeight();
			camera.camera->SetProjection(-camera.aspect_ratio * camera.current_zoom,
				camera.aspect_ratio * camera.current_zoom, -camera.current_zoom, camera.current_zoom);
		}
	}
}
