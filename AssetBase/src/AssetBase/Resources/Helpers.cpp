#include "pch.h"
#include "Helpers.h"

#include <iostream>
#include <sstream>

namespace AssetBase
{
	float Helpers::StringToFloat(const std::string& string, const std::string& field)
	{
		try {
			return std::stof(string);
		}
		catch (std::invalid_argument const&) {
			throw std::stringstream("Invalid value for field '") << field << "'.\n";
		}
		catch (std::out_of_range const&) {
			throw "Float out of range in field '" + field + "'.";
		}
	}

	int Helpers::StringToInt(const std::string& string, const std::string& field)
	{
		try {
			return std::stoi(string);
		}
		catch (std::invalid_argument const&) {
			throw std::stringstream("Invalid value for field '") << field << "'.\n";
		}
		catch (std::out_of_range const&) {
			throw "Float out of range in field '" + field + "'.";
		}
	}
}

