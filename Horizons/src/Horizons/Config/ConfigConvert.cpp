#include "pch.h"
#include "ConfigConvert.h"

#include "Lithium.h"

std::string BoolToString(bool value)
{
	if (value)
		return "true";
	return "false";
}

std::string VariantToString(const ConfigStore::Variant& variant)
{
	if (std::holds_alternative<bool>(variant))
		return BoolToString(std::get<bool>(variant));
	else if (std::holds_alternative<int>(variant))
		return std::to_string(std::get<int>(variant));
	else if (std::holds_alternative<float>(variant))
		return std::to_string(std::get<float>(variant));
	else if (std::holds_alternative<std::string>(variant))
		return std::get<std::string>(variant);
	return "ERROR";
}

bool ConfigVariantSetFromString(ConfigStore::Variant& variant, const std::string& value)
{
	if (std::holds_alternative<bool>(variant))
	{
		if (value == "true")
		{
			variant = true;
			return true;
		}
		else if (value == "false")
		{
			variant = true;
			return true;
		}
		else return false;
	}
	else if (std::holds_alternative<int>(variant))
	{
		try {
			variant = std::stoi(value);
			return true;
		}
		catch (std::invalid_argument) {
			return false;
		}
		catch (std::out_of_range) {
			return false;
		}
	}
	else if (std::holds_alternative<float>(variant))
	{
		try {
			variant = std::stof(value);
			return true;
		}
		catch (const std::invalid_argument&) {
			return false;
		}
		catch (const std::out_of_range&) {
			return false;
		}
	}
	else if (std::holds_alternative<std::string>(variant))
	{
		variant = value;
		return true;
	}
	return false;
}
