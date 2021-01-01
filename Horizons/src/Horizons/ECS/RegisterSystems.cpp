#include "pch.h"
#include "RegisterSystems.h"

#include "Horizons/Gameplay/RNGSystem.h"

#include "Horizons/UI/UIClickSystem.h"
#include "Horizons/UI/UIHoverSystem.h"
#include "Horizons/UI/UILayoutSystem.h"
#include "Horizons/UI/UITransformSortSystem.h"
#include "Horizons/UI/UIRenderSystem.h"

#include "Horizons/Rendering/FlickerSystem.h"

void RegisterSystems(SystemRegistry& registry)
{
	// Gameplay //
	registry.Register("RNG")
		.SetOnInit(Systems::RNG::OnInit);

	// Rendering //
	registry.Register("Flicker")
		.SetOnUpdate(Systems::Flicker::OnUpdate);

	// UI //
	registry.Register("UIClick")
		.SetOnEvent(Systems::UIClick::OnEvent);

	registry.Register("UIHover")
		.SetOnEvent(Systems::UIHover::OnEvent);

	registry.Register("UILayout")
		.SetOnEvent(Systems::UILayout::OnEvent)
		.SetOnUpdate(Systems::UILayout::OnUpdate)
		.SetOnDeinit(Systems::UILayout::OnDeinit);

	registry.Register("UISortTransforms")
		.SetOnUpdate(Systems::UISortTransforms::OnUpdate);
	
	registry.Register("UIRender")
		.SetOnRender(Systems::UIRender::OnRender);
}
