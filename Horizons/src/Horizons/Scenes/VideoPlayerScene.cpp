#include "pch.h"
#ifndef LI_DIST
#include "VideoPlayerScene.h"

#include "Horizons.h"
#include "MainMenuScene.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <algorithm>

//////////////////////
// VideoPlayerLayer //
//////////////////////

VideoPlayerLayer::VideoPlayerLayer()
	: Layer("VideoPlayerLayer"), m_Exit(false)
{
	LI_ASSERT(m_VideoPlayer.Open("data/videos/bowling.mkv"), "Failed to open video!");
}

VideoPlayerLayer::~VideoPlayerLayer()
{
}

void VideoPlayerLayer::OnAttach()
{
}

void VideoPlayerLayer::OnDetach()
{
}

void VideoPlayerLayer::OnUpdate(li::duration::us dt)
{
	li::Window* window = li::Application::Get()->GetWindow();
	window->GetContext()->BindDefaultRenderTarget();
	window->GetContext()->Clear();
	
	li::Renderer::BeginUI();
	if (m_VideoPlayer.UpdateFrame(dt))
	{
		float window_width = static_cast<float>(window->GetWidth());
		float window_height = static_cast<float>(window->GetHeight());
		float scale = std::min(window->GetWidth() / (float)m_VideoPlayer.GetWidth(), window->GetHeight() / (float)m_VideoPlayer.GetHeight());
		float scaled_width = scale * m_VideoPlayer.GetWidth();
		float scaled_height = scale * m_VideoPlayer.GetHeight();

		li::Renderer::UISubmit(m_VideoPlayer.GetTexture(), glm::translate(glm::mat4(1.0f), { (window_width - scaled_width) / 2, (window_height - scaled_height) / 2, 1.0f })
			* glm::scale(glm::mat4(1.0f), { scaled_width, scaled_height, 1.0f }));
	}
	li::Renderer::EndUI();
}

void VideoPlayerLayer::OnEvent(SDL_Event* event)
{
	if (event->type == SDL_KEYUP && event->key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		m_Exit = true;
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
	Horizons* app = li::Application::Get<Horizons>();
	app->PopLayer(&m_VideoPlayerLayer);
#ifndef LI_DIST
	app->PopOverlay(&m_Diagnostics);
#endif
}

void VideoPlayerScene::TransitionIn()
{
	Horizons* app = li::Application::Get<Horizons>();
	app->PushLayer(&m_VideoPlayerLayer);
#ifndef LI_DIST
	app->PushOverlay(&m_Diagnostics);
	app->GetImGuiRenderer()->SetBlockEvents(true);
#endif
}

void VideoPlayerScene::TransitionOut()
{
}

void VideoPlayerScene::OnUpdate(li::duration::us dt)
{
	if (m_VideoPlayerLayer.ReturnToMainMenu())
	{
		li::Application::Get()->Transition(new MainMenuScene());
	}
}
#endif
