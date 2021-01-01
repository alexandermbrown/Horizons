#pragma once

#include "Lithium/Core/Core.h"

namespace Li
{
	class Scene
	{
	public:
		virtual ~Scene() = default;

		virtual void OnShow() = 0;
		virtual void OnTransition() = 0;
	};
}
