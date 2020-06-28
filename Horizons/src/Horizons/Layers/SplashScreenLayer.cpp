#include "pch.h"
#include "SplashScreenLayer.h"

#include "glm/gtc/matrix_transform.hpp"
#include "Horizons/Layers/GameLayer.h"

SplashScreenLayer::SplashScreenLayer()
{
	m_Texture = li::Texture2D::Create("data/textures/horizons_splash.png", 
		li::WrapType::ClampToEdge, li::WrapType::ClampToEdge,
		li::FilterType::Linear, li::FilterType::Linear);

	CalculateTransform();

	li::ResourceManager::LoadAsync("data/resources.lab");
}

void SplashScreenLayer::OnAttach()
{
}

void SplashScreenLayer::OnDetach()
{
	li::Ref<li::Window>& window = li::Application::Get()->GetWindow();
	window->SetBordered(true);
	window->SetResizable(true);
	window->SetSize(1280, 720);
	window->SetPosition(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

void SplashScreenLayer::OnUpdate(float dt)
{
	if (!li::ResourceManager::DequeueAsset() && li::ResourceManager::IsLoaded())
	{
		li::ResourceManager::PrintInfo();

		// TODO: make config.
		li::Localization::SetLocale("en-us");

		li::Application* app = li::Application::Get();
		app->PushLayer(new GameLayer());
		app->PopLayer(this);
	}

	li::RendererAPI::Clear();
	li::Renderer::UISubmit(m_Texture, glm::vec4(1.0f), m_Transform);
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
	li::Ref<li::Window>& window = li::Application::Get()->GetWindow();

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
