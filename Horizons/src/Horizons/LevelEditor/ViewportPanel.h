#pragma once

#include "Lithium.h"
#include "SDL.h"
#include "glm/glm.hpp"
#include "entt/entt.hpp"

#include "Horizons/Terrain/TerrainRenderer.h"
#include "EditorTerrainStore.h"
#include "EditorSettings.h"

class ViewportPanel
{
public:
	ViewportPanel(EditorSettings* settings);
	~ViewportPanel();

	inline void ShowPanel() { m_WindowOpen = true; }
	void OnUpdate(float dt);
	void RenderPanel();
	void OnEvent(SDL_Event* event);

	bool FileNew(const std::string& path, glm::ivec2 world_size);
	bool FileOpen(const std::string& path);
	void FileSave();
	void FileSaveAs(const std::string& path);

	void CloseTerrain();

	inline bool IsTerrainOpen() const { return m_TerrainOpen; }
	inline glm::ivec2 GetWorldSize() const { return m_TerrainStore.GetWorldSize(); }
	inline bool IsTerrainModified() const { return m_TerrainStore.IsModified(); }

private:
	entt::registry m_Registry;

	EditorSettings* m_Settings;
	bool m_WindowOpen;
	bool m_ViewportFocused;
	bool m_ViewportHovered;
	glm::ivec2 m_MousePos;
	glm::ivec2 m_ViewportSize;
	li::Ref<li::Framebuffer> m_ViewportFB;

	bool m_TerrainOpen;

	EditorTerrainStore m_TerrainStore;
	TerrainRenderer m_TerrainRenderer;

	glm::ivec2 m_ChunkCenter;
	glm::vec2 m_CameraFocus;
};