#include "lipch.h"
#include "OrthographicCamera.h"

#include "Lithium/Core/Application.h"
#include "glm/gtc/matrix_transform.hpp"

namespace li 
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ViewMatrix(1.0f)
	{
		switch (Application::Get()->GetAPI())
		{
		case RendererAPI::OpenGL:
			m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -100.0f, 1.0f);
			break;
		case RendererAPI::D3D11:
			m_ProjectionMatrix = glm::orthoLH_ZO(left, right, bottom, top, 100.0f, -1.0f);
			break;
		}

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		switch (Application::Get()->GetAPI())
		{
		case RendererAPI::OpenGL:
			m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -100.0f, 1.0f);
			break;
		case RendererAPI::D3D11:
			m_ProjectionMatrix = glm::orthoLH_ZO(left, right, bottom, top, 100.0f, -1.0f);
			break;
		}

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}