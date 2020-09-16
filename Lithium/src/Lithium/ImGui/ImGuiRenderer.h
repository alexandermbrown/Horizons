#pragma once
#ifndef LI_DIST
#include "Lithium/Core/Core.h"
#include "Lithium/Core/Layer.h"

#include "imgui.h"
#include "SDL.h"

namespace li 
{
	class ImGuiRenderer
	{
	public:
		ImGuiRenderer();
		virtual ~ImGuiRenderer() = default;

		virtual void Begin() = 0;
		virtual void End() = 0;
		void OnEvent(SDL_Event* event);
		void Resize(int width, int height);
		inline void SetBlockEvents(bool block) { m_BlockEvents = block; }

		static Ref<ImGuiRenderer> Create();

		static void InitImGuiStyle();

	private:
		bool m_BlockEvents;
	};
}
#endif