#include "pch.h"
#include "UIFunctions.h"

#include "Horizons.h"
#include "Horizons/UI/UILayoutSystem.h"
#include "Horizons/UI/UIComponents.h"
#include "Horizons/Rendering/RenderingComponents.h"

#include "layout/layout.h"
#include "entt/entt.hpp"

void UIFunctions::LoadFunctions(sol::state& lua)
{
	auto UI = lua["UI"].get_or_create<sol::table>();

	UI.new_enum<lay_box_flags>("Contain", {
		{ "Row", LAY_ROW },
		{ "Column", LAY_COLUMN },
		{ "Layout", LAY_LAYOUT },
		{ "Flex", LAY_FLEX },
		{ "NoWrap", LAY_NOWRAP },
		{ "Wrap", LAY_WRAP },
		{ "Start", LAY_START },
		{ "Middle", LAY_MIDDLE },
		{ "End", LAY_END },
		{ "Justify", LAY_JUSTIFY }
	});

	UI.new_enum<lay_layout_flags>("Behave", {
		{ "Left", LAY_LEFT },
		{ "Top", LAY_TOP },
		{ "Right", LAY_RIGHT },
		{ "Bottom", LAY_BOTTOM },
		{ "HFill", LAY_HFILL },
		{ "VFill", LAY_VFILL },
		{ "HCenter", LAY_HCENTER },
		{ "VCenter", LAY_VCENTER },
		{ "Center", LAY_CENTER },
		{ "Fill", LAY_FILL },
		{ "Break", LAY_BREAK }
	});

	UI.set_function("CreateUIContext", [](sol::light<entt::registry> registry_ptr, float z_start, float z_range) -> entt::entity
	{
		entt::registry& registry = registry_ptr;

		entt::entity context_id = registry.create();
		auto& context = registry.emplace<cp::ui_context>(context_id);
		context.start_z = z_start;
		context.z_range = z_range;
		// Attach root element to context.
		registry.emplace<cp::ui_element>(context_id);
		
		return context_id;
	});

	UI.set_function("UIContextRebuild", [](sol::light<entt::registry> registry_ptr, entt::entity context_id)
	{
		(*registry_ptr).get<cp::ui_context>(context_id).rebuild = true;
	});

	UI.set_function("UIContextRecalculate", [](sol::light<entt::registry> registry_ptr, entt::entity context_id)
	{
		(*registry_ptr).get<cp::ui_context>(context_id).recalculate = true;
	});


	UI.set_function("CreateUIElement", [](sol::light<entt::registry> registry_ptr) -> entt::entity
	{
		entt::registry& registry = registry_ptr;
		entt::entity entity = registry.create();
		registry.emplace<cp::ui_transform>(entity);
		registry.emplace<cp::ui_element>(entity);
		return entity;
	});

	UI.set_function("UIElementAddChild", [](sol::light<entt::registry> registry_ptr, entt::entity parent_id, entt::entity child_id)
	{
		Systems::UILayout::AddChild(*registry_ptr, parent_id, child_id);
	});

	UI.set_function("UIElementSetSize", [](sol::light<entt::registry> registry_ptr, entt::entity element_id, int width, int height)
	{
		cp::ui_element& element = (*registry_ptr).get<cp::ui_element>(element_id);
		element.width = width;
		element.height = height;
	});

	UI.set_function("UIElementSetMargins", [](sol::light<entt::registry> registry_ptr, entt::entity element_id, sol::table margins)
	{
		cp::ui_element& element = (*registry_ptr).get<cp::ui_element>(element_id);
		
		sol::optional<int> left = margins["left"];
		sol::optional<int> top = margins["top"];
		sol::optional<int> right = margins["right"];
		sol::optional<int> bottom = margins["bottom"];

		element.margins = lay_vec4_xyzw(left.value_or(0), top.value_or(0), right.value_or(0), bottom.value_or(0));
	});

	UI.set_function("UIElementSetContain", [](sol::light<entt::registry> registry_ptr, entt::entity element, sol::variadic_args vargs)
	{
		uint32_t flags = 0;
		for (auto arg : vargs)
		{
			if (arg.is<lay_box_flags>())
				flags |= arg.as<lay_box_flags>();
			else
				LI_ERROR("Expected UI.Contain enum in argument list but found {}",
					sol::type_name(arg.lua_state(), arg.get_type()));
		}

		(*registry_ptr).get<cp::ui_element>(element).layout_contain = flags;
	});

	UI.set_function("UIElementSetBehave", [](sol::light<entt::registry> registry_ptr, entt::entity element, sol::variadic_args vargs)
	{
		uint32_t flags = 0;
		for (auto arg : vargs)
		{
			if (arg.is<lay_layout_flags>())
				flags |= arg.as<lay_layout_flags>();
			else
				LI_ERROR("Expected UI.Behave enum in argument list but found {}",
					sol::type_name(arg.lua_state(), arg.get_type()));
		}

		(*registry_ptr).get<cp::ui_element>(element).layout_behave = flags;
	});

	UI.set_function("UIElementSetColor", [](sol::light<entt::registry> registry_ptr, entt::entity element, float red, float green, float blue, float alpha)
	{
		(*registry_ptr).emplace_or_replace<cp::color>(element, glm::vec4{ red, green, blue, alpha });
	});

	UI.set_function("UIElementSetTexture", [](sol::light<entt::registry> registry_ptr, entt::entity element, std::string texture)
	{
		(*registry_ptr).emplace_or_replace<cp::texture>(element, std::move(texture));
	});

	UI.set_function("UIElementSetLabel", [](sol::light<entt::registry> registry_ptr, entt::entity element, sol::table label_data)
	{
		sol::optional<const char*> text = label_data["text"];
		sol::optional<int> pt_size = label_data["pt_size"];
		sol::optional<std::string> font = label_data["font"];
		sol::optional<bool> dynamic = label_data["dynamic"];
		sol::optional<int> excess = label_data["excess"];

		if (text && pt_size && font)
		{
			cp::label& label_cp = (*registry_ptr).emplace<cp::label>(element);
			label_cp.label_ref = Li::MakeRef<Li::Label>(text.value(), pt_size.value(), Li::ResourceManager::Get<Li::Font>(font.value()), dynamic.value_or(false), excess.value_or(0));
		}
		else LI_ERROR("Missing label data in UIElementSetLabel(..), entity {}", element);
	});

	UI.set_function("UIElementAddTextureCrop", [](sol::light<entt::registry> registry_ptr, entt::entity element)
	{
		(*registry_ptr).emplace<cp::ui_texture_crop>(element);
	});

	UI.set_function("UIElementAddTextureFlicker", [](sol::light<entt::registry> registry_ptr, entt::entity element, float magnitude, float delay)
	{
		entt::registry& registry = registry_ptr;
		registry.emplace<cp::flicker>(element, magnitude,
				Li::Timer(Li::Duration::Cast<Li::Duration::us>(Li::Duration::fsec(delay)), false, true));

		registry.emplace_or_replace<cp::color>(element, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
	});

	UI.set_function("UIElementSetEvents", [](sol::light<entt::registry> registry_ptr, entt::entity element, sol::table handlers)
	{
		entt::registry& registry = registry_ptr;

		sol::optional<sol::function> OnMouseEnter = handlers["OnMouseEnter"];
		sol::optional<sol::function> OnMouseLeave = handlers["OnMouseLeave"];
		if (OnMouseEnter || OnMouseLeave)
		{
			cp::ui_hover& hover = registry.emplace<cp::ui_hover>(element);
			if (OnMouseEnter)
				hover.OnMouseEnterLuaFn = OnMouseEnter.value();
			if (OnMouseLeave)
				hover.OnMouseLeaveLuaFn = OnMouseLeave.value();
		}

		sol::optional<sol::function> OnMouseDown = handlers["OnMouseLeave"];
		sol::optional<sol::function> OnMouseUp = handlers["OnMouseLeave"];
		sol::optional<sol::function> OnClick = handlers["OnClick"];
		if (OnMouseDown || OnMouseUp || OnClick)
		{
			cp::ui_click& click = registry.emplace<cp::ui_click>(element);
			if (OnMouseDown)
				click.OnMouseDownLuaFn = OnMouseDown.value();
			if (OnMouseUp)
				click.OnMouseUpLuaFn = OnMouseUp.value();
			if (OnClick)
				click.OnClickLuaFn = OnClick.value();
		}
	});
}
