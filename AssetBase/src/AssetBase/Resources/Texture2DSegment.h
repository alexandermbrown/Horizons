#pragma once

#include "flatbuffers/flatbuffers.h"
#include "lab_serial/assets_generated.h"
#include "yaml-cpp/yaml.h"

#include <filesystem>

namespace AssetBase
{
	flatbuffers::Offset<Assets::Texture2D> SerializeTexture2D(flatbuffers::FlatBufferBuilder& builder, const std::filesystem::path& base_path, const std::string& name, YAML::Node texture, bool debug_mode);
}
