#include "lipch.h"
#include "UI.h"

#include "Lithium/Core/Application.h"
#include "Lithium/Renderer/Renderer.h"

#include "glm/gtc/matrix_transform.hpp"

namespace li
{
	Scope<UI::UIData> UI::s_Data;

	void UI::Init()
	{
		s_Data = CreateScope<UIData>();

		lay_init_context(&s_Data->Context);

		lay_reserve_items_capacity(&s_Data->Context, MaxItemCount);
	}

	void UI::Shutdown()
	{
		lay_destroy_context(&s_Data->Context);
		s_Data.reset();
	}

	void UI::Begin()
	{
		lay_reset_context(&s_Data->Context);

		LI_CORE_ASSERT(s_Data->ElementStack.size() == 0, "Element stack is not empty.");

		s_Data->Root = lay_item(&s_Data->Context);
		s_Data->ElementStack.push(s_Data->Root);

		s_Data->RenderInfo[s_Data->Root] = nullptr;

		Ref<Window>& window = Application::Get()->GetWindow();
		lay_set_size_xy(&s_Data->Context, s_Data->Root, (lay_scalar)window->GetWidth(), (lay_scalar)window->GetHeight());
	}

	void UI::End()
	{
		s_Data->ElementStack.pop();

		LI_CORE_ASSERT(s_Data->ElementStack.size() == 0, "Element stack is not empty. Did you forget to call EndChild()?");

		lay_run_context(&s_Data->Context);

		s_Data->ZIncrement = (s_Data->ZTop - s_Data->ZBottom) / (float)MaxItemCount;

		RenderChildren(s_Data->Root, s_Data->ZBottom);
	}

	void UI::BeginChild(UIRenderInfo* info)
	{
		lay_id element = lay_item(&s_Data->Context);
		lay_insert(&s_Data->Context, s_Data->ElementStack.top(), element);
		s_Data->ElementStack.push(element);
		s_Data->RenderInfo[element] = info;
	}

	uint32_t UI::EndChild()
	{
		uint32_t child = s_Data->ElementStack.top();
		s_Data->ElementStack.pop();
		return child;
	}

	void UI::SetSize(lay_scalar width, lay_scalar height)
	{
		lay_set_size_xy(&s_Data->Context, s_Data->ElementStack.top(), width, height);
	}

	void UI::SetMargins(lay_scalar left, lay_scalar top, lay_scalar right, lay_scalar bottom)
	{
		lay_set_margins_ltrb(&s_Data->Context, s_Data->ElementStack.top(), left, top, right, bottom);
	}

	void UI::SetBehave(uint32_t flags)
	{
		lay_set_behave(&s_Data->Context, s_Data->ElementStack.top(), flags);
	}

	void UI::SetContain(uint32_t flags)
	{
		lay_set_contain(&s_Data->Context, s_Data->ElementStack.top(), flags);
	}

	void UI::RenderChildren(lay_id parent, float z)
	{
		for (lay_id child = lay_first_child(&s_Data->Context, parent); child != LAY_INVALID_ID; child = lay_next_sibling(&s_Data->Context, child))
		{
			UIRenderInfo* info = s_Data->RenderInfo[child];

			if (info)
			{
				lay_vec4 rect = lay_get_rect(&s_Data->Context, child);

				if (info->Label)
				{
					glm::mat4 transform = glm::translate(glm::mat4(1.0f),
						{
							(float)rect[0],
							(float)(Application::Get()->GetWindow()->GetHeight() - rect[1] - rect[3]),
							z });
					Renderer::UISubmitLabel(info->Label, transform, info->Color);
				}
				else
				{
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), { (float)rect[0],
							(float)(Application::Get()->GetWindow()->GetHeight() - rect[1] - rect[3]), z })
							* glm::scale(glm::mat4(1.0f), { (float)rect[2], (float)rect[3], 1.0f });
					if (!info->TextureAlias.empty())
					{
						Renderer::UISubmitColoredTexture(info->TextureAlias, info->Color, transform);
					}
					else
					{
						Renderer::UISubmitColored(info->Color, transform);
					}
				}
			}

			RenderChildren(child, z + s_Data->ZIncrement);
		}
	}
}