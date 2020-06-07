#include "pch.h"
#include "OrthographicCameraController.h"

#include "Lithium/Core/Core.h"
#include "Lithium/Core/Input.h"
#include <SDL.h>

OrthographicCameraController::OrthographicCameraController(float aspectRatio, float zoom, bool rotation)
	: m_AspectRatio(aspectRatio), 
	m_ZoomLevel(zoom),
	m_Camera(-m_AspectRatio * zoom, m_AspectRatio * zoom, -zoom, zoom), m_Rotation(rotation),
	m_CameraRotationSpeed((float)M_PI / 2.0f)
{
}

void OrthographicCameraController::OnUpdate(float dt)
{
	bool doPosition = false;
	if (li::Input::IsKeyPressed(SDLK_a))
	{
		m_CameraPosition.x -= SDL_cosf(m_CameraRotation) * m_CameraTranslationSpeed * dt;
		m_CameraPosition.y -= SDL_sinf(m_CameraRotation) * m_CameraTranslationSpeed * dt;
		doPosition = true;
	}
	else if (li::Input::IsKeyPressed(SDLK_d))
	{
		m_CameraPosition.x += SDL_cosf(m_CameraRotation) * m_CameraTranslationSpeed * dt;
		m_CameraPosition.y += SDL_sinf(m_CameraRotation) * m_CameraTranslationSpeed * dt;
		doPosition = true;
	}

	if (li::Input::IsKeyPressed(SDLK_w))
	{
		m_CameraPosition.x += -SDL_sinf(m_CameraRotation) * m_CameraTranslationSpeed * dt;
		m_CameraPosition.y += SDL_cosf(m_CameraRotation) * m_CameraTranslationSpeed * dt;
		doPosition = true;
	}
	else if (li::Input::IsKeyPressed(SDLK_s))
	{
		m_CameraPosition.x -= -SDL_sinf(m_CameraRotation) * m_CameraTranslationSpeed * dt;
		m_CameraPosition.y -= SDL_cosf(m_CameraRotation) * m_CameraTranslationSpeed * dt;
		doPosition = true;
	}

	if (m_Rotation)
	{
		bool doRotation = false;
		if (li::Input::IsKeyPressed(SDLK_q))
		{
			m_CameraRotation += m_CameraRotationSpeed * dt;
			doRotation = true;
		}
		if (li::Input::IsKeyPressed(SDLK_e))
		{
			m_CameraRotation -= m_CameraRotationSpeed * dt;
			doRotation = true;
		}

		if (doRotation)
			m_Camera.SetRotation(m_CameraRotation);
	}
	
	if (doPosition)
		m_Camera.SetPosition(m_CameraPosition);

	m_CameraTranslationSpeed = m_ZoomLevel;
}

void OrthographicCameraController::OnEvent(SDL_Event* event)
{
	li::EventDispatcher dispatcher(event);
	dispatcher.Dispatch(SDL_MOUSEWHEEL, LI_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
	dispatcher.Dispatch(SDL_WINDOWEVENT, LI_BIND_EVENT_FN(OrthographicCameraController::OnWindowEvent));
}

void OrthographicCameraController::OnMouseScrolled(SDL_Event* event)
{
	m_ZoomLevel -= event->wheel.y * 0.25f;
	m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
	m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
}

void OrthographicCameraController::OnWindowEvent(SDL_Event* event)
{
	if (event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
		int w, h;
		SDL_GL_GetDrawableSize(li::Application::Get().GetWindow()->GetWindow(), &w, &h);
		m_AspectRatio = (float)w / (float)h;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}
}
