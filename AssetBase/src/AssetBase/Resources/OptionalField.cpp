#include "pch.h"
#include "OptionalField.h"

#include <stdexcept>

const std::string& AssetBase::GetOptionalString(YAML::Node node, const char* field)
{
	YAML::Node path_node = node[field];
	if (path_node && path_node.IsScalar())
		return path_node.Scalar();
	else
		throw std::runtime_error("Field " + std::string(field) + " was not found or is not a string.\n");
}
