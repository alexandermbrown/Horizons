#include "pch.h"
#include "Prototypes.h"

#include "Horizons/UI/UILayoutSystem.h"
#include "Horizons/UI/UIComponents.h"
#include "Horizons/Rendering/RenderingComponents.h"

li::Scope<Prototypes::PrototypeData> Prototypes::s_Data = nullptr;

void Prototypes::Init(const std::string& lua_path)
{
	LI_ASSERT(s_Data == nullptr, "Instance already created!");

	s_Data = li::CreateScope<PrototypeData>();

	s_Data->Lua.open_libraries(sol::lib::base, sol::lib::jit, sol::lib::package, sol::lib::bit32);

	try {
		sol::protected_function_result result = s_Data->Lua.script_file(lua_path);
		if (!result.valid())
		{
			LI_ERROR("Script Error!");
		}
	}
	catch (...)
	{
		LI_ERROR("Script Error!");
	}
}

void Prototypes::InstantiateUIPrototype(entt::registry& registry, entt::entity parent, const std::string& name, int recursion_limit)
{
	auto prototypes = s_Data->Lua["prototypes"];
	if (!prototypes.valid())
	{
		LI_ERROR("Error reading global 'prototypes'");
		return;
	}

	auto ui = prototypes["ui"];
	if (!ui.valid())
	{
		LI_ERROR("Error reading 'prototypes.ui'");
		return;
	}

	sol::optional<sol::table> root = ui[name];
	if (!root)
	{
		LI_ERROR("Error reading 'prototypes.ui.{}'", name);
		return;
	}

	UILoadElement(registry, parent, root.value(), name, recursion_limit);
}

void Prototypes::UILoadElement(entt::registry& registry, entt::entity parent, const sol::table& element, const std::string& prototype, int recursion_limit)
{
	if (recursion_limit <= 0)
	{
		LI_ERROR("UI recursion limit reached, some elements may not appear.");
		return;
	}

	sol::optional<std::string> prototype_template = element["prototype"];
	if (prototype_template)
	{
		InstantiateUIPrototype(registry, parent, prototype_template.value(), recursion_limit - 1);
		return;
	}

	// Load current element.
	entt::entity entity = registry.create();
	registry.emplace<cp::ui_transform>(entity);

	// Layout.
	{
		auto& layout = registry.emplace<cp::ui_element>(entity);

		sol::optional<int> width = element["width"];
		if (width)
			layout.width = width.value();

		sol::optional<int> height = element["height"];
		if (height)
			layout.height = height.value();

		sol::optional<sol::table> margins = element["margins"];
		if (margins)
		{
			sol::table& margins_table = margins.value();
			sol::optional<int> left		= margins_table["left"];
			sol::optional<int> top		= margins_table["top"];
			sol::optional<int> right	= margins_table["right"];
			sol::optional<int> bottom	= margins_table["bottom"];

			layout.margins = lay_vec4_xyzw(left.value_or(0), top.value_or(0), right.value_or(0), bottom.value_or(0));
		}

		sol::optional<uint32_t> layout_contain = element["layout_contain"];
		if (layout_contain)
			layout.layout_contain = layout_contain.value();

		sol::optional<uint32_t> layout_behave = element["layout_behave"];
		if (layout_behave)
			layout.layout_behave = layout_behave.value();
	}
	// Texture.
	sol::optional<std::string> texture = element["texture"];
	if (texture)
	{
		registry.emplace<cp::texture>(entity, texture.value());
	}
	// Color.
	sol::optional<sol::table> color = element["color"];
	if (color)
	{
		auto& color_table = color.value();
		auto& color_cp = registry.emplace<cp::color>(entity);
		sol::optional<float> red	= color_table[1];
		sol::optional<float> green	= color_table[2];
		sol::optional<float> blue	= color_table[3];
		sol::optional<float> alpha	= color_table[4];

		if (red && green && blue && alpha)
			color_cp.color = { red.value(), green.value(), blue.value(), alpha.value() };
		else
			LI_ERROR("Invalid color in ui prototype {}", prototype);
	}
	// Label.
	sol::optional<sol::table> label = element["label"];
	if (label)
	{
		auto& label_table = label.value();
		auto& label_cp = registry.emplace<cp::label>(entity);
		sol::optional<const char*> text = label_table["text"];
		sol::optional<int> pt_size		= label_table["pt_size"];
		sol::optional<std::string> font = label_table["font"];

		if (text && pt_size && font)
			label_cp.label_ref = li::CreateRef<li::Label>(text.value(), pt_size.value(), li::ResourceManager::Get<li::Font>(font.value()), false);
		else
			LI_ERROR("Invalid font in ui prototype {}", prototype);
	}
	// Mouse move events.
	sol::optional<sol::function> OnMouseEnter = element["OnMouseEnter"];
	sol::optional<sol::function> OnMouseLeave = element["OnMouseLeave"];
	if (OnMouseEnter || OnMouseLeave)
	{
		auto& hover = registry.emplace<cp::ui_hover>(entity);
		if (OnMouseEnter)
			hover.OnMouseEnterLuaFn = OnMouseEnter.value();
		if (OnMouseLeave)
			hover.OnMouseLeaveLuaFn = OnMouseLeave.value();
	}
	// Mouse click events.
	sol::optional<sol::function> OnMouseDown = element["OnMouseDown"];
	sol::optional<sol::function> OnMouseUp = element["OnMouseUp"];
	sol::optional<sol::function> OnClick = element["OnClick"];
	if (OnMouseDown || OnMouseUp || OnClick)
	{
		auto& click = registry.emplace<cp::ui_click>(entity);
		if (OnMouseDown)
			click.OnMouseDownLuaFn = OnMouseDown.value();
		if (OnMouseUp)
			click.OnMouseUpLuaFn = OnMouseUp.value();
		if (OnClick)
			click.OnClickLuaFn = OnClick.value();
	}

	UILayoutSystem::AddChild(registry, parent, entity);

	// Load children.
	sol::optional<sol::table> children = element["children"];
	if (children)
	{
		auto children_value = children.value();
		for (int i = 1; i <= children_value.size(); i++)
		{
			sol::optional<sol::table> child = children_value[i];
			if (child)
			{
				UILoadElement(registry, entity, child.value(), prototype, recursion_limit);
			}
			else
			{
				LI_ERROR("Invalid child in ui prototype {} at index {}.", prototype, i);
			}
		}
	}
}
