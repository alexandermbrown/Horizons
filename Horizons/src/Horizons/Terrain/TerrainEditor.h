#pragma once

#ifndef LI_DIST
#include "Horizons/Core/Core.h"
#include <string>

class TerrainEditor
{
public:
	TerrainEditor();

	void OpenTerrain(const std::string& filepath);
	void SaveToDisk();

	void ApplyBrush(float inner_radius, float outer_radius, float offset);

private:


};
#endif // !LI_DIST
