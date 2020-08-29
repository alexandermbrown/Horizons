#pragma once

#include "PrototypeData.h"

#include "Lithium.h"
#include "entt/entt.hpp"

class UIPrototypes
{
public:
	static void Init(li::Ref<PrototypeData> data);
	static void InstantiateUIPrototype(entt::registry& registry, entt::entity parent, const std::string& name, int recursion_limit = 32);
	static void LoadFunctions();

private:
	static void UILoadElement(entt::registry& registry, entt::entity parent, const sol::table& element, const std::string& prototype, int recursion_limit);

	static li::Ref<PrototypeData> s_Data;
};