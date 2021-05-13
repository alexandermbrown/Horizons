#pragma once

#include "Resources/FontSerializer.h"
#include "yaml-cpp/yaml.h"

#include <vector>
#include <string>

class AssetBase
{
public:
	AssetBase();

	const FontSerializer& GetFontSerializer() const { return m_FontSerializer; }
	const std::vector<std::string>& GetIncludePaths() const { return m_IncludePaths; }
	inline static AssetBase* Get() { return s_Instance; }

private:
	void CompileAssetPack(const std::string& yaml_path, const std::string& out_path, bool debug_mode);

	FontSerializer m_FontSerializer;
	std::vector<std::string> m_IncludePaths;

	static AssetBase* s_Instance;
};
