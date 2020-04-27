#include "lipch.h"
#include "Lithium/Renderer/OrthographicCameraController.h"

#include "Lithium/Core/Core.h"
#include "Lithium/Core/Input.h"
#include <SDL.h>

namespace li
{
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
		if (Input::IsKeyPressed(SDLK_a))
		{
			m_CameraPosition.x -= SDL_cos(m_CameraRotation) * m_CameraTranslationSpeed * dt;
			m_CameraPosition.y -= SDL_sin(m_CameraRotation) * m_CameraTranslationSpeed * dt;
			doPosition = true;
		}
		else if (Input::IsKeyPressed(SDLK_d))
		{
			m_CameraPosition.x += SDL_cos(m_CameraRotation) * m_CameraTranslationSpeed * dt;
			m_CameraPosition.y += SDL_sin(m_CameraRotation) * m_CameraTranslationSpeed * dt;
			doPosition = true;
		}

		if (Input::IsKeyPressed(SDLK_w))
		{
			m_CameraPosition.x += -SDL_sin(m_CameraRotation) * m_CameraTranslationSpeed * dt;
			m_CameraPosition.y += SDL_cos(m_CameraRotation) * m_CameraTranslationSpeed * dt;
			doPosition = true;
		}
		else if (Input::IsKeyPressed(SDLK_s))
		{
			m_CameraPosition.x -= -SDL_sin(m_CameraRotation) * m_CameraTranslationSpeed * dt;
			m_CameraPosition.y -= SDL_cos(m_CameraRotation) * m_CameraTranslationSpeed * dt;
			doPosition = true;
		}

		if (m_Rotation)
		{
			bool doRotation = false;
			if (Input::IsKeyPressed(SDLK_q))
			{
				m_CameraRotation += m_CameraRotationSpeed * dt;
				doRotation = true;
			}
			if (Input::IsKeyPressed(SDLK_e))
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

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(LI_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(LI_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}