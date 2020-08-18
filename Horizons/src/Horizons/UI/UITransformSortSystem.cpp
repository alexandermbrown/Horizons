#include "pch.h"
#include "UITransformSortSystem.h"

#include "UIComponents.h"

void UITransformSortSystem::SortTransforms(entt::registry& registry)
{
	registry.sort<cp::ui_transform>([](const cp::ui_transform& lhs, const cp::ui_transform& rhs)
	{
		return lhs.transform[3][2] < rhs.transform[3][2];
	});
}
