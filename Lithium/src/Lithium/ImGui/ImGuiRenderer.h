#pragma once
#ifndef LI_DIST
#include "Lithium/Core/Memory.h"
#include "Lithium/Core/Layer.h"

#include "imgui.h"
#include "SDL.h"

namespace Li 
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

		static Unique<ImGuiRenderer> Create();

		static void InitImGuiStyle();

	private:
		bool m_BlockEvents;
	};
}
#endif
