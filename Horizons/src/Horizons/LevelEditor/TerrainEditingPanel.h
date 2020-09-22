#pragma once
#ifndef LI_DIST

#include "EditorSettings.h"

class TerrainEditingPanel
{
public:
	TerrainEditingPanel(EditorSettings* settings);

	void OnImGuiRender();
	inline void ShowPanel() { m_WindowOpen = true; }

private:
	bool m_WindowOpen;
	EditorSettings* m_Settings;
};

#endif