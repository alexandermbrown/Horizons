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

	const float half_zoom = camera.current_zoom * 0.5f;
	camera.camera = new li::OrthographicCamera(-camera.aspect_ratio * half_zoom,
		camera.aspect_ratio * half_zoom, -half_zoom, half_zoom);
	camera.camera->SetLookAt({ 0.0f, -3.0f, std::sqrt(3.0f) }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
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

	for (auto&& [entity, transform, player] : registry.view<cp::transform, cp::player>().proxy())
	{
		glm::vec3 camera_pos = {
			transform.position.x,
			transform.position.y - 3.0f,
			transform.position.z + std::sqrt(3.0f),
		};
		if (camera.camera->GetPosition() != camera_pos)
		{
			camera.camera->SetLookAt(camera_pos, transform.position, { 0.0f, 0.0f, 1.0f });
		}
		break;
	}
	
	if (!camera.finished_zoom)
	{
		float delta = camera.current_zoom - camera.target_zoom;
		if (dt < 0.99f)
			camera.current_zoom -= delta * dt * camera.zoom_speed;
		else
			camera.current_zoom = camera.target_zoom;

		const float half_zoom = camera.current_zoom * 0.5f;
		camera.camera->SetProjection(-camera.aspect_ratio * half_zoom,
			camera.aspect_ratio * half_zoom, -half_zoom, half_zoom);

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
			const float half_zoom = camera.current_zoom * 0.5f;
			camera.camera->SetProjection(-camera.aspect_ratio * half_zoom,
				camera.aspect_ratio * half_zoom, -half_zoom, half_zoom);
		}
	}
}
