#pragma once

#include "flatbuffers/flatbuffers.h"
#include "lab_serial/assets_generated.h"
#include "yaml-cpp/yaml.h"

#include <filesystem>

flatbuffers::Offset<Assets::TextureAtlas> SerializeTextureAtlas(flatbuffers::FlatBufferBuilder& builder, const std::filesystem::path& base_path, const std::string& name, YAML::Node atlas, bool debug_mode);
