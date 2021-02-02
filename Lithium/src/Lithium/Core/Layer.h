#pragma once

#include "Lithium/Utility/Time.h"
#include "SDL.h"

namespace Li
{
	class Layer
	{
	public:
		Layer(const std::string& name);
		virtual ~Layer() = default;

		virtual void OnUpdate(Duration::us dt) {}
		virtual void OnEvent(SDL_Event* event) {}
#ifndef LI_DIST
		virtual void OnImGuiRender() {}
#endif
		inline const std::string& GetName() const { return m_Name; }

	protected:
		std::string m_Name;
	};
}
