#include "pch.h"
#ifndef LI_DIST
#include "TerrainEditor.h"

TerrainEditor::TerrainEditor()
{
}

void TerrainEditor::OpenTerrain(const std::string& filepath)
{
	m_CurrentPath = filepath;
}

void TerrainEditor::SaveToDisk()
{
}

void TerrainEditor::ApplyBrush(glm::vec2 position, float inner_radius, float outer_radius, float offset)
{
}
#endif // !LI_DIST
