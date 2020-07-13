#include "pch.h"
#include "TransformUpdateSystem.h"

#include "glm/gtc/matrix_transform.hpp"

void TransformUpdateSystem::Update(entt::registry& registry)
{
	registry.view<cp::transform>().each([](cp::transform& transform) {
		if (transform.old)
		{
			transform.transform = glm::translate(glm::mat4(1.0f), transform.position);

			if (transform.rotation != 0.0f)
				transform.transform *= glm::rotate(glm::mat4(1.0f), transform.rotation, { 0.0f, 0.0f, 1.0f });

			transform.transform *= glm::scale(glm::mat4(1.0f), transform.scale);

			transform.old = false;
		}
	});
}
