#pragma once
#include "Lithium.h"
#ifndef LI_DIST

class VideoPlayerLayer : public Li::Layer
{
public:
	VideoPlayerLayer();
	virtual ~VideoPlayerLayer() = default;

	virtual void OnUpdate(Li::Duration::us dt) override;
	virtual void OnEvent(SDL_Event* event) override;
#ifndef LI_DIST
	virtual void OnImGuiRender() override;
#endif

private:
	Li::AVPlayer m_VideoPlayer;
};

class VideoPlayerScene : public Li::Scene
{
public:
	VideoPlayerScene();
	virtual ~VideoPlayerScene();

	virtual void OnShow() override;
	virtual void OnTransition() override;
};
#endif
