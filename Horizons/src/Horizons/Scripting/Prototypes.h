#pragma once

#include "Lithium.h"

#ifdef LI_DEBUG
#define SOL_ALL_SAFETIES_ON 1
#endif

#include "sol.hpp"
#include "entt/entt.hpp"

class Prototypes
{
public:

	static void Init(const std::string& lua_path);

	static void InstantiateUIPrototype(entt::registry& registry, entt::entity parent, const std::string& name, int recursion_limit = 32);

private:
	static void UILoadElement(entt::registry& registry, entt::entity parent, const sol::table& element, const std::string& prototype, int recursion_limit);

	struct PrototypeData
	{
		sol::state Lua;
	};

	static li::Scope<PrototypeData> s_Data;
};