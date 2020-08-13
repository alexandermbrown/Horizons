#pragma once

#include "Lithium/Core/Core.h"

#include "Lithium/ImGui/ImGuiRenderer.h"

namespace li
{
	class D3D11ImGuiRenderer : public ImGuiRenderer
	{
	public:
		D3D11ImGuiRenderer();
		virtual ~D3D11ImGuiRenderer() override;

		virtual void Begin() override;
		virtual void End() override;

	private:


	};
}