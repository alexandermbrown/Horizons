#pragma once
#ifndef LI_DIST

#include "Brush.h"

class TerrainEditingPanel
{
public:
	TerrainEditingPanel(Brush* brush);

	void OnImGuiRender();
	inline void ShowPanel() { m_WindowOpen = true; }

private:
	Brush* m_Brush;
	bool m_WindowOpen;
};

#endif