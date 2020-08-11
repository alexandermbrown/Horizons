#include "pch.h"
#include "SplashScreenLayer.h"

#include "glm/gtc/matrix_transform.hpp"


SplashScreenLayer::SplashScreenLayer()
{
	m_Texture = li::ResourceManager::Get<li::Texture2D>("texture_horizons_splash");

	CalculateTransform();
}

void SplashScreenLayer::OnAttach()
{
}

void SplashScreenLayer::OnDetach()
{
}

void SplashScreenLayer::OnUpdate(float dt)
{
	li::RendererAPI::Clear();
	li::Renderer::UISubmit(m_Texture, m_Transform);
}

void SplashScreenLayer::OnImGuiRender()
{
}

void SplashScreenLayer::OnEvent(SDL_Event* event)
{
	if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
	{
		CalculateTransform();
	}
}

void SplashScreenLayer::CalculateTransform()
{
	li::Window* window = li::Application::Get()->GetWindow();

	float windowAspect = (float)window->GetWidth() / (float)window->GetHeight();
	float textureAspect = (float)m_Texture->GetWidth() / (float)m_Texture->GetHeight();

	if (windowAspect >= textureAspect)
	{
		m_Transform = glm::translate(glm::mat4(1.0f), 
			{ 
				0.0f,
				0.5f * (window->GetHeight() - window->GetWidth() / textureAspect),
				0.0f
			})
			* glm::scale(glm::mat4(1.0f),
				{
					window->GetWidth(),
					window->GetWidth() / textureAspect,
					1.0f
				});
	}
	else
	{
		m_Transform = glm::translate(glm::mat4(1.0f), 
			{
				0.5f * (window->GetWidth() - window->GetHeight() * textureAspect),
				0.0f,
				0.0f
			})
			* glm::scale(glm::mat4(1.0f),
				{
					window->GetHeight() * textureAspect,
					window->GetHeight(),
					1.0f
				});
	}
}
