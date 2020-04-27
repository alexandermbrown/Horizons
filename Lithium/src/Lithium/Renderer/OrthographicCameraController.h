#pragma once

#include "Lithium/Renderer/OrthographicCamera.h"

#include "Lithium/Events/Event.h"
#include "Lithium/Events/WindowEvent.h"
#include "Lithium/Events/MouseEvent.h"

namespace li
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, float zoom = 1.0f, bool rotation = false);

		void OnUpdate(float ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed;
	};

}