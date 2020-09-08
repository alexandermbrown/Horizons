#pragma once
#ifndef LI_DIST

#include "Horizons/LevelEditor/ViewportPanel.h"

#include "Lithium.h"
#include "entt/entt.hpp"
#include "glm/glm.hpp"

class LevelEditorLayer : public li::Layer
{
public:
	LevelEditorLayer();
	virtual ~LevelEditorLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(float dt) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(SDL_Event* event) override;

	inline bool ReturnToMainMenu() const { return m_ReturnToMainMenu; }

private:
	bool m_ReturnToMainMenu;

	bool m_DockspaceOpen;

	ViewportPanel m_Viewport;
};
#endif // !LI_DIST
