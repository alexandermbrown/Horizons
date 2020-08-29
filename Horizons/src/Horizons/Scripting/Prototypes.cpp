#include "pch.h"
#include "Prototypes.h"

#include "UIPrototypes.h"
#include "TerrainPrototypes.h"
#include "Horizons/Rendering/RenderingComponents.h"

#include "entt/entt.hpp"

li::Ref<PrototypeData> Prototypes::s_Data = nullptr;

void Prototypes::Init(const std::string& lua_path)
{
	LI_ASSERT(s_Data == nullptr, "Instance already created!");

	s_Data = li::CreateRef<PrototypeData>();

	s_Data->Lua.open_libraries(sol::lib::base, sol::lib::jit, sol::lib::package, sol::lib::bit32);
	
	UIPrototypes::Init(s_Data);
	TerrainPrototypes::Init(s_Data);
	LoadFunctions();

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

void Prototypes::Shutdown()
{
	TerrainPrototypes::Shutdown();
	s_Data.reset();
}

void Prototypes::LoadFunctions()
{
	s_Data->Lua.set_function("SetColor", [](sol::light<entt::registry> registry, entt::entity entity, sol::table color)
	{
		sol::optional<float> red = color[1];
		sol::optional<float> green = color[2];
		sol::optional<float> blue = color[3];
		sol::optional<float> alpha = color[4];

		registry.value->emplace_or_replace<cp::color>(entity, glm::vec4(
			red.value_or(1.0f), green.value_or(1.0f), blue.value_or(1.0f), alpha.value_or(1.0f)));
	});
	
	s_Data->Lua.set_function("SetTexture", [](sol::light<entt::registry> registry, entt::entity entity, std::string texture)
	{
		registry.value->emplace_or_replace<cp::texture>(entity, texture);
	});

	s_Data->Lua.set_function("SetLabel", [](sol::light<entt::registry> registry, entt::entity entity, sol::table label)
	{
		if (registry.value->has<cp::label>(entity))
		{
			sol::optional<const char*> text = label["text"];

			if (text)
			{
				auto& label_cp = registry.value->get<cp::label>(entity);
				label_cp.label_ref->SetText(text.value());
			}
			else LI_ERROR("'text' not found!");
		}
		else
		{
			auto& label_cp = registry.value->emplace<cp::label>(entity);
			sol::optional<const char*> text	= label["text"];
			sol::optional<int> pt_size		= label["pt_size"];
			sol::optional<std::string> font = label["font"];

			if (text && pt_size && font)
				label_cp.label_ref = li::CreateRef<li::Label>(text.value(), pt_size.value(), li::ResourceManager::Get<li::Font>(font.value()), false);
			else
				LI_ERROR("Invalid font specification.");
		}
	});

	UIPrototypes::LoadFunctions();
}