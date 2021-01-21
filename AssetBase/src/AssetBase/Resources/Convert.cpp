#include "pch.h"
#include "Convert.h"

#include <stdexcept>
#include <iostream>
#include <sstream>

namespace AssetBase::Convert
{
	float StringToFloat(const std::string& string, const std::string& field)
	{
		try {
			return std::stof(string);
		}
		catch (const std::invalid_argument&) {
			throw std::runtime_error("Invalid value for field " + field);
		}
		catch (const std::out_of_range&) {
			throw std::runtime_error("Float out of range in field " + field);
		}
	}

	int StringToInt(const std::string& string, const std::string& field)
	{
		try {
			return std::stoi(string);
		}
		catch (const std::invalid_argument&) {
			throw std::runtime_error("Invalid value for field " + field);
		}
		catch (const std::out_of_range&) {
			throw std::runtime_error("Float out of range in field " + field);
		}
	}
}
