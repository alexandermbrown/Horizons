#pragma once
#ifndef LI_DIST

#include "glm/glm.hpp"
#include <string>

class TerrainEditor
{
public:
	TerrainEditor();

	void OpenTerrain(const std::string& filepath);
	void SaveToDisk();

	void ApplyBrush(glm::vec2 position, float inner_radius, float outer_radius, float offset);

private:
	std::string m_CurrentPath;

};
#endif // !LI_DIST
