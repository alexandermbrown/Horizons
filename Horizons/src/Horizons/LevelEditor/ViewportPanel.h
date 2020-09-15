#pragma once

#include "Lithium.h"
#include "SDL.h"
#include "glm/glm.hpp"
#include "entt/entt.hpp"

#include "Horizons/Terrain/TerrainRenderer.h"
#include "EditorTerrainStore.h"

class ViewportPanel
{
public:
	ViewportPanel();
	~ViewportPanel();

	inline void OpenWindow() { m_WindowOpen = true; }
	void OnUpdate(float dt);
	void OnImGuiRender();
	void OnEvent(SDL_Event* event);

	void FileOpen(const std::string& path);

private:
	entt::registry m_Registry;

	bool m_WindowOpen;
	bool m_ViewportFocused;
	bool m_ViewportHovered;
	glm::ivec2 m_MousePos;
	glm::ivec2 m_ViewportSize;
	li::Ref<li::Framebuffer> m_ViewportFB;

	bool m_TerrainOpen;

	EditorTerrainStore m_TerrainStore;
	TerrainRenderer m_TerrainRenderer;
};