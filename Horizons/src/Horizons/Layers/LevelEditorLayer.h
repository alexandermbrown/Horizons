#pragma once
#ifndef LI_DIST

#include "Horizons/LevelEditor/ViewportPanel.h"
#include "Horizons/LevelEditor/TerrainEditingPanel.h"

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

	Brush m_Brush;
	ViewportPanel m_Viewport;
	TerrainEditingPanel m_TerrainEditingPanel;
};
#endif // !LI_DIST
