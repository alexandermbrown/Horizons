#include "pch.h"
#include "FlickerSystem.h"

#include "RenderingComponents.h"
#include <random>

void Systems::Flicker::OnUpdate(entt::registry& registry, Li::Duration::us dt)
{
	auto& rand = registry.ctx<std::mt19937>();

	registry.view<cp::color, cp::flicker>().each([dt, &rand](entt::entity entity, cp::color& color, cp::flicker& flicker)
	{
		std::uniform_real_distribution<float> dist(1.0f - flicker.magnitude, 1.0f);

		float value = flicker.current + (flicker.target - flicker.current) * flicker.timer.GetFraction();
		color.color.r = value;
		color.color.g = value;
		color.color.b = value;
		if (flicker.timer.Update(dt))
		{
			flicker.current = value;
			flicker.target = dist(rand);
		}
	});
}
