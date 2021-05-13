#pragma once

#include "flatbuffers/flatbuffers.h"
#include "lab_serial/assets_generated.h"
#include "yaml-cpp/yaml.h"

#include "msdfgen/msdfgen.h"
#include "msdfgen/msdfgen-ext.h"

#include <filesystem>

class FontSerializer
{
public:
	FontSerializer();
	~FontSerializer();

	flatbuffers::Offset<Assets::Font> Serialize(flatbuffers::FlatBufferBuilder& builder, const std::filesystem::path& base_path, const std::string& name, YAML::Node atlas, bool debug_mode) const;

private:
	msdfgen::FreetypeHandle* ft = nullptr;
};
