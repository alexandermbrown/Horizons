#pragma once

#include "yaml-cpp/yaml.h"

const std::string& GetOptionalString(YAML::Node node, const char* field);
