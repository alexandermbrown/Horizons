#pragma once

#include "glm/glm.hpp"
#include "readerwriterqueue/readerwriterqueue.h"

namespace cp
{
	struct sync_transform
	{
		uint64_t sync_id = 0;

		glm::vec3 position{ 0.0f, 0.0f, 0.0f };
		glm::vec2 velocity{ 0.0f, 0.0f };

		float rotation = 0.0f;
		float angular_velocity = 0.0f;

		bool operator==(const sync_transform& other)
		{
			return sync_id == other.sync_id
				&& position == other.position
				&& velocity == other.velocity
				&& rotation == other.rotation
				&& angular_velocity == other.angular_velocity;
		}

		bool operator!=(const sync_transform& other)
		{
			return sync_id != other.sync_id
				|| position != other.position
				|| velocity != other.velocity
				|| rotation != other.rotation
				|| angular_velocity != other.angular_velocity;
		}
	};
}

using SyncTransformQueue = moodycamel::ReaderWriterQueue<cp::sync_transform>;