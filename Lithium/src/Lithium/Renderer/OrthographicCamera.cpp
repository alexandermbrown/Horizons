#include "lipch.h"
#include "OrthographicCamera.h"

#include "Lithium/Core/Application.h"
#include "glm/gtc/matrix_transform.hpp"

namespace li 
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ViewMatrix(1.0f)
	{
		SetProjection(left, right, bottom, top);
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		switch (Application::Get()->GetAPI())
		{
		case RendererAPI::OpenGL:
			m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -100.0f, 100.0f);
			break;
		case RendererAPI::D3D11:
			m_ProjectionMatrix = glm::orthoRH_ZO(left, right, bottom, top, -100.0f, 100.0f);
			break;
		}
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		RecalculateViewMatrix();
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetLookAt(const glm::vec3& camera_pos, const glm::vec3& target_pos, const glm::vec3& up)
	{
		m_Position = camera_pos;
		m_ViewMatrix = glm::lookAt(camera_pos, target_pos, up);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
		m_ViewMatrix = glm::inverse(transform);
	}
}