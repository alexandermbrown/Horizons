#include "pch.h"
#ifndef LI_DIST
#include "EditorCameraSystem.h"

#include "Horizons/Rendering/RenderingComponents.h"
#include "EditorComponents.h"

void EditorCameraSystem::Init(entt::registry& registry)
{
	cp::camera& camera = registry.set<cp::camera>();

	camera.current_zoom = 10.0f;
	camera.target_zoom = 10.0f;
	camera.aspect_ratio = 2.0f;

	const float half_zoom = camera.current_zoom * 0.5f;
	camera.camera = new li::OrthographicCamera(-camera.aspect_ratio * half_zoom,
		camera.aspect_ratio * half_zoom, -half_zoom, half_zoom);
	camera.camera->SetLookAt({ 0.0f, -3.0f, std::sqrt(3.0f) }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
	camera.min_zoom = 0.01f;
	camera.max_zoom = 100.0f;
	camera.zoom_speed = 20.0f;

	registry.set<cp::camera_pan>();
}

void EditorCameraSystem::Shutdown(entt::registry& registry)
{
	cp::camera& camera = registry.ctx<cp::camera>();
	LI_ASSERT(camera.camera, "Camera should not be nullptr.");
	delete camera.camera;
}

void EditorCameraSystem::Update(entt::registry& registry, li::duration::us dt)
{
	cp::camera& camera = registry.ctx<cp::camera>();
	if (!camera.finished_zoom)
	{
		float delta = camera.current_zoom - camera.target_zoom;
		if (dt < li::duration::ms(990))
			camera.current_zoom -= delta * li::duration::fsec(dt).count() * camera.zoom_speed;
		else
			camera.current_zoom = camera.target_zoom;

		const float half_zoom = camera.current_zoom * 0.5f;
		camera.camera->SetProjection(-camera.aspect_ratio * half_zoom,
			camera.aspect_ratio * half_zoom, -half_zoom, half_zoom);

		if (std::abs(delta) < 1e-5f)
		{
			camera.finished_zoom = true;
			camera.current_zoom = camera.target_zoom;
		}
	}
}

void EditorCameraSystem::OnEvent(entt::registry& registry, SDL_Event* event, glm::ivec2 view_size)
{
	if (event->type == SDL_MOUSEWHEEL)
	{
		cp::camera& camera = registry.ctx<cp::camera>();

		camera.target_zoom -= event->wheel.y * camera.target_zoom / 4.0f;

		if (camera.target_zoom < camera.min_zoom)
			camera.target_zoom = camera.min_zoom;
		else if (camera.target_zoom > camera.max_zoom)
			camera.target_zoom = camera.max_zoom;

		camera.finished_zoom = false;
	}
	else if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		cp::camera_pan& pan = registry.ctx<cp::camera_pan>();
		if (event->button.button == pan.button)
		{
			SDL_SetRelativeMouseMode(SDL_TRUE);
			pan.started = true;
			pan.start_pos = { event->button.x, event->button.y };
		}
	}
	else if (event->type == SDL_MOUSEBUTTONUP)
	{
		cp::camera_pan& pan = registry.ctx<cp::camera_pan>();
		if (event->button.button == pan.button)
		{
			SDL_SetRelativeMouseMode(SDL_FALSE);
			pan.started = false;
		}
	}
	else if (event->type == SDL_MOUSEMOTION)
	{
		cp::camera_pan& pan = registry.ctx<cp::camera_pan>();
		if (pan.started)
		{
			cp::camera& camera = registry.ctx<cp::camera>();
			glm::vec3 offset = {
				-event->motion.xrel * camera.aspect_ratio * camera.current_zoom / (float)view_size.x,
				event->motion.yrel * 2.0f * camera.current_zoom / (float)view_size.y,
				0.0f
			};

			const glm::vec3& camera_pos = camera.camera->GetPosition() + offset;
			camera.camera->SetLookAt(camera_pos, camera_pos + glm::vec3{ 0.0f, 3.0f, -std::sqrt(3.0f) }, { 0.0f, 0.0f, 1.0f });

			SDL_WarpMouseInWindow(SDL_GetMouseFocus(), pan.start_pos.x, pan.start_pos.y);
		}
	}
}

void EditorCameraSystem::Resize(entt::registry& registry, int width, int height)
{
	cp::camera& camera = registry.ctx<cp::camera>();

	camera.aspect_ratio = (float)width / (float)height;
	const float half_zoom = camera.current_zoom * 0.5f;
	camera.camera->SetProjection(-camera.aspect_ratio * half_zoom,
		camera.aspect_ratio * half_zoom, -half_zoom, half_zoom);
}

glm::vec2 EditorCameraSystem::GetCameraFocusPoint(entt::registry& registry)
{
	cp::camera& camera = registry.ctx<cp::camera>();
	const glm::vec3& camera_pos = camera.camera->GetPosition();
	return { camera_pos.x, camera_pos.y + 3.0f };
}
#endif
