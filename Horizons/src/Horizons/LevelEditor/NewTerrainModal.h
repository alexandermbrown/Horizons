#pragma once
#ifndef LI_DIST

#include "glm/glm.hpp"

class NewTerrainModal
{
public:
	NewTerrainModal();
	void RenderModal();

	inline void Open() { m_ImGuiOpenPopup = true; }

	bool IsDone();
	inline const std::string& GetPath() const { return m_Path; }
	inline glm::ivec2 GetWorldSize() const { return m_WorldSize; }
	

private:
	void FileDialog();

	bool m_IsDone;
	bool m_ImGuiOpenPopup;
	std::string m_Path;
	glm::ivec2 m_WorldSize;

	const char* m_WindowTitle = "New Terrain File";
};

#endif
