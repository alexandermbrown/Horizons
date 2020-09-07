#pragma once

#include "Lithium/Core/Core.h"
#include "SDL.h"

namespace li
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float dt) {}
		virtual void OnEvent(SDL_Event* event) {}
#ifndef LI_DIST
		virtual void OnImGuiRender() {}
#endif
		inline const std::string GetName() const { return m_DebugName; }

	private:
		std::string m_DebugName;
	};
}
