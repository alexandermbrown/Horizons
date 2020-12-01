#pragma once
#include "Lithium.h"
#ifndef LI_DIST

#include "Horizons/Layers/DiagnosticsLayer.h"

class VideoPlayerLayer : public li::Layer
{
public:
	VideoPlayerLayer();
	virtual ~VideoPlayerLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(li::duration::us dt) override;
	virtual void OnEvent(SDL_Event* event) override;
#ifndef LI_DIST
	virtual void OnImGuiRender() override;
#endif

	inline bool ReturnToMainMenu() const { return m_Exit; }

private:
	li::AVPlayer m_VideoPlayer;
	bool m_Exit;
};

class VideoPlayerScene : public li::Scene
{
public:
	VideoPlayerScene();
	virtual ~VideoPlayerScene();

	virtual void TransitionIn() override;
	virtual void TransitionOut() override;

	virtual void OnUpdate(li::duration::us dt) override;

	virtual bool Finished() override { return true; }

private:
	VideoPlayerLayer m_VideoPlayerLayer;
#ifndef LI_DIST
	DiagnosticsLayer m_Diagnostics;
#endif
};
#endif