#pragma once

#include "ConfigStore.h"
#include <string>

std::string BoolToString(bool value);
std::string VariantToString(const ConfigStore::Variant& variant);

bool ConfigVariantSetFromString(ConfigStore::Variant& variant, const std::string& value);
