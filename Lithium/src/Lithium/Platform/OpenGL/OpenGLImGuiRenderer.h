#pragma once
#ifndef LI_DIST
#include "Lithium/ImGui/ImGuiRenderer.h"

namespace Li
{
	class OpenGLImGuiRenderer : public ImGuiRenderer
	{
	public:
		OpenGLImGuiRenderer();
		virtual ~OpenGLImGuiRenderer() override;

		virtual void Begin() override;
		virtual void End() override;
	};
}
#endif
