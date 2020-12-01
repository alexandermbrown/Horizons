#pragma once

namespace li
{
	class Scene
	{
	public:
		virtual ~Scene() = default;

		virtual void TransitionIn() = 0;
		virtual void TransitionOut() = 0;

		virtual void OnUpdate(li::duration::us dt) = 0;

		virtual bool Finished() = 0;
	};
}
