#pragma once

#include "flatbuffers/flatbuffers.h"
#include "lab_serial/assets_generated.h"
#include "yaml-cpp/yaml.h"

#include <filesystem>

namespace AssetBase
{
	void InitFreetype();
	void ShutdownFreetype();

	flatbuffers::Offset<Assets::Font> SerializeFont(flatbuffers::FlatBufferBuilder& builder, const std::filesystem::path& base_path, const std::string& name, YAML::Node atlas, bool debug_mode);
}
