#pragma once

#include "yaml-cpp/yaml.h"

namespace AssetBase
{
	const std::string& GetOptionalString(YAML::Node node, const char* field);
}
