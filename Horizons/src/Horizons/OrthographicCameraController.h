#pragma once

#include "Lithium/Renderer/OrthographicCamera.h"

#include "Lithium.h"

class OrthographicCameraController
{
public:
	OrthographicCameraController(float aspectRatio, float zoom = 1.0f, bool rotation = false);

	void OnUpdate(float ts);
	void OnEvent(SDL_Event* event);

	li::OrthographicCamera& GetCamera() { return m_Camera; }
	const li::OrthographicCamera& GetCamera() const { return m_Camera; }

	float GetZoomLevel() const { return m_ZoomLevel; }
	void SetZoomLevel(float level) { m_ZoomLevel = level; }
private:
	void OnMouseScrolled(SDL_Event* event);
	void OnWindowEvent(SDL_Event* event);
private:
	float m_AspectRatio;
	float m_ZoomLevel;
	li::OrthographicCamera m_Camera;

	bool m_Rotation;

	glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
	float m_CameraRotation = 0.0f;
	float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed;
};