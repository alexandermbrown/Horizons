#pragma once
#ifndef LI_DIST

#include "Horizons/LevelEditor/ViewportPanel.h"
#include "Horizons/LevelEditor/TerrainEditingPanel.h"
#include "Horizons/LevelEditor/NewTerrainModal.h"

#include "Lithium.h"
#include "entt/entt.hpp"
#include "glm/glm.hpp"
#include "SDL.h"

#include <filesystem>

class LevelEditorLayer : public Li::Layer
{
public:
	LevelEditorLayer();
	virtual ~LevelEditorLayer();

	virtual void OnUpdate(Li::Duration::us dt) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(SDL_Event* event) override;

private:
	void FileNew();
	void FileOpen();
	void FileOpenDialog();
	void FileSave();
	void FileSaveAs();

	void UnsavedChangesDialog(int* button_id);
	
	bool m_DockspaceOpen;
	bool m_TitleHasAsterisk;

	EditorSettings m_Settings;
	ViewportPanel m_Viewport;
	TerrainEditingPanel m_TerrainEditingPanel;
	NewTerrainModal m_NewTerrainModal;

	std::filesystem::path m_TerrainPath;

	const SDL_MessageBoxColorScheme m_MsgBoxColorScheme = { {
			/* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
			{ 255,   0,   0 },
			/* [SDL_MESSAGEBOX_COLOR_TEXT] */
			{   0, 255,   0 },
			/* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
			{ 255, 255,   0 },
			/* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
			{   0,   0, 255 },
			/* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
			{ 255,   0, 255 }
	} };

	const SDL_MessageBoxButtonData m_OverwriteButtons[3] = {
		{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Save" },
		{                                       0, 1, "Don't Save" },
		{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "Cancel" }
	};
};
#endif // !LI_DIST
