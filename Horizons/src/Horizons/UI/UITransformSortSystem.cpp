#include "pch.h"
#include "UITransformSortSystem.h"

#include "UIComponents.h"

void Systems::UISortTransforms::OnUpdate(entt::registry& registry, Li::Duration::us dt)
{
	registry.sort<cp::ui_transform>([](const cp::ui_transform& lhs, const cp::ui_transform& rhs)
	{
		return lhs.transform[3][2] < rhs.transform[3][2];
	});
}
