#include "pch.h"
#ifndef LI_DIST
#include "VideoPlayerScene.h"

#include "Horizons.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Horizons/Scripting/ScriptScene.h"

//////////////////////
// VideoPlayerLayer //
//////////////////////

VideoPlayerLayer::VideoPlayerLayer()
	: Layer("VideoPlayer")
{
	LI_VERIFY(m_VideoPlayer.Open("data/videos/bowling.mkv"), "Failed to open video!");
}

void VideoPlayerLayer::OnUpdate(Li::Duration::us dt)
{
	Li::Window& window = Li::Application::Get().GetWindow();
	
	Li::Renderer::BeginUI();
	if (m_VideoPlayer.UpdateFrame(dt))
	{
		float window_width = static_cast<float>(window.GetWidth());
		float window_height = static_cast<float>(window.GetHeight());
		float scale = std::min(window_width / (float)m_VideoPlayer.GetWidth(), window_height / (float)m_VideoPlayer.GetHeight());
		float scaled_width = scale * m_VideoPlayer.GetWidth();
		float scaled_height = scale * m_VideoPlayer.GetHeight();

		Li::Renderer::UISubmit(m_VideoPlayer.GetTexture(), glm::translate(glm::mat4(1.0f), { (window_width - scaled_width) / 2, (window_height - scaled_height) / 2, 1.0f })
			* glm::scale(glm::mat4(1.0f), { scaled_width, scaled_height, 1.0f }));
	}
	Li::Renderer::EndUI();
}

void VideoPlayerLayer::OnEvent(SDL_Event* event)
{
	if (event->type == SDL_KEYUP && event->key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		Li::Application::Get().Transition(Li::MakeUnique<ScriptScene>("MainMenuScene"), true);
}

void VideoPlayerLayer::OnImGuiRender()
{
}

//////////////////////
// VideoPlayerScene //
//////////////////////

VideoPlayerScene::VideoPlayerScene()
{
}

VideoPlayerScene::~VideoPlayerScene()
{
	Horizons& app = Li::Application::Get<Horizons>();
	app.PopLayer("VideoPlayer");
}

void VideoPlayerScene::OnShow()
{
	Horizons& app = Li::Application::Get<Horizons>();
	app.PushLayer(Li::MakeUnique<VideoPlayerLayer>());
#ifndef LI_DIST
	app.GetImGuiRenderer()->SetBlockEvents(true);
#endif
}

void VideoPlayerScene::OnTransition()
{
}
#endif
