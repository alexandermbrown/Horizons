#pragma once

#include "Lithium/Core/Core.h"
#include "Lithium/Renderer/Font.h"

#include "layout/layout.h"
#include "glm/glm.hpp"

#include <unordered_map>
#include <stack>

namespace li
{
	struct UIRenderInfo
	{
		std::string TextureAlias = "";
		Ref<Label> Label = nullptr;
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	};

	class UI
	{
	public:

		static void Init();
		static void Shutdown();

		static void Begin();
		static void End();

		// Set info to nullptr for an invisible element.
		static void BeginChild(UIRenderInfo* info);

		static uint32_t EndChild();

		static void SetSize(lay_scalar width, lay_scalar height);

		static void SetMargins(lay_scalar left, lay_scalar top, lay_scalar right, lay_scalar bottom);

		static void SetBehave(uint32_t flags);

		static void SetContain(uint32_t flags);

		static inline void SetZ(float z_bottom, float z_top)
		{
			s_Data->ZBottom = z_bottom;
			s_Data->ZTop = z_top;
		}

	private:

		static void RenderChildren(lay_id parent, float z);

		static const lay_id MaxItemCount = 1024;

		struct UIData
		{
			lay_context Context;
			lay_id Root;

			UIRenderInfo* RenderInfo[MaxItemCount];

			std::stack<lay_id> ElementStack;

			float ZBottom = 0.5f;
			float ZTop = 0.6f;
			float ZIncrement = 0.0f;

			int Click;
		};

		static Scope<UIData> s_Data;
	};
}