#pragma once

#include <string>

namespace AssetBase
{

	class Helpers
	{
	public:
		static float StringToFloat(const std::string& string, const std::string& field);
		static int StringToInt(const std::string& string, const std::string& field);
	};

}