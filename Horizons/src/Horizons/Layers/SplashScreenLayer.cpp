#include "pch.h"
#include "SplashScreenLayer.h"

#include "Horizons/Scripting/ScriptScene.h"

#include "Horizons.h"
#include "glm/gtc/matrix_transform.hpp"

SplashScreenLayer::SplashScreenLayer()
	: Layer("SplashScreen")
{
	m_Texture = Li::ResourceManager::Get<Li::Texture2D>("texture_horizons_splash");

	CalculateTransform();
}

void SplashScreenLayer::OnUpdate(Li::Duration::us dt)
{
	Li::Application::Get().GetWindow().GetContext()->Clear();

	if (!Li::ResourceManager::UpdateStaggeredLoad())
	{
		Horizons& app = Li::Application::Get<Horizons>();

		Li::ResourceManager::PrintInfo();

		// TODO: Make the renderer not dependant on the resource manager
		// Instead, change the below function to set the shaders used for instancing, fonts, etc.
		// In future, move the ResourceManager into Horizons and remove any references in Lithium.
		Li::Renderer::InitPostResourceLoad();
		Li::Renderer::AddTextureAtlas(Li::ResourceManager::Get<Li::TextureAtlas>("atlas_test"));
		Li::Renderer::AddTextureAtlas(Li::ResourceManager::Get<Li::TextureAtlas>("space_scene_1"));
		Li::Renderer::AddTextureAtlas(Li::ResourceManager::Get<Li::TextureAtlas>("atlas_terrain_earth"));

		app.GetTerrainData().LoadTypes();

		// TODO: Get locale from config.
		Li::Localization::SetLocale("en-us");

		app.Transition(Li::MakeUnique<ScriptScene>("MainMenuScene"), true);
	}
	Li::Renderer::UISubmit(m_Texture, m_Transform);
}
#ifndef LI_DIST
void SplashScreenLayer::OnImGuiRender()
{
}
#endif
void SplashScreenLayer::OnEvent(SDL_Event* event)
{
	if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
	{
		CalculateTransform();
	}
}

void SplashScreenLayer::CalculateTransform()
{
	Li::Window& window = Li::Application::Get().GetWindow();

	float windowAspect = (float)window.GetWidth() / (float)window.GetHeight();
	float textureAspect = (float)m_Texture->GetWidth() / (float)m_Texture->GetHeight();

	if (windowAspect >= textureAspect)
	{
		m_Transform = glm::translate(glm::mat4(1.0f), { 
				0.0f,
				0.5f * (window.GetHeight() - window.GetWidth() / textureAspect),
				0.0f
			}) * glm::scale(glm::mat4(1.0f), {
				window.GetWidth(),
				window.GetWidth() / textureAspect,
				1.0f
			});
	}
	else
	{
		m_Transform = glm::translate(glm::mat4(1.0f), {
				0.5f * (window.GetWidth() - window.GetHeight() * textureAspect),
				0.0f,
				0.0f
			}) * glm::scale(glm::mat4(1.0f), {
				window.GetHeight() * textureAspect,
				window.GetHeight(),
				1.0f
			});
	}
}
