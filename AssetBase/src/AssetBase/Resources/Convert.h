#pragma once

#include <string>

namespace AssetBase::Convert
{
	float StringToFloat(const std::string& string, const std::string& field);
	int StringToInt(const std::string& string, const std::string& field);
}
