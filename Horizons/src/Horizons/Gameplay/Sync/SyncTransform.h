#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "readerwriterqueue/readerwriterqueue.h"

namespace cp
{
	struct sync_transform
	{
		uint64_t sync_id = 0;

		glm::vec3 position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 velocity{ 0.0f, 0.0f, 0.0f };

		glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
		glm::quat angular_velocity{ 1.0f, 0.0f, 0.0f, 0.0f };

		bool is_upright = false;

		bool operator==(const sync_transform& other)
		{
			return sync_id == other.sync_id
				&& position == other.position
				&& velocity == other.velocity
				&& rotation == other.rotation
				&& angular_velocity == other.angular_velocity
				&& is_upright == other.is_upright;
		}

		bool operator!=(const sync_transform& other)
		{
			return sync_id != other.sync_id
				|| position != other.position
				|| velocity != other.velocity
				|| rotation != other.rotation
				|| angular_velocity != other.angular_velocity
				|| is_upright != other.is_upright;
		}
	};
}

using SyncTransformQueue = moodycamel::ReaderWriterQueue<cp::sync_transform>;