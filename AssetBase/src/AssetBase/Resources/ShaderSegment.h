#pragma once

#include "flatbuffers/flatbuffers.h"
#include "lab_serial/assets_generated.h"
#include "yaml-cpp/yaml.h"

#include <filesystem>

namespace AssetBase
{
	flatbuffers::Offset<Assets::Shader> SerializeShader(flatbuffers::FlatBufferBuilder& builder, const std::filesystem::path& base_path, const std::string& name, YAML::Node shader, bool debug_mode);
}
