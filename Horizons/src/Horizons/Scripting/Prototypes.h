#pragma once

#include "PrototypeData.h"
#include "Lithium.h"

class Prototypes
{
public:
	static void Init(const std::string& lua_path);
	static void Shutdown();

private:
	static void LoadFunctions();

	static li::Ref<PrototypeData> s_Data;
};