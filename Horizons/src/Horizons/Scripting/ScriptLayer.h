#pragma once

#include "Horizons/ECS/ECS.h"
#include "Lithium.h"
#include "sol.hpp"

class ScriptLayer : public Li::Layer
{
public:
	ScriptLayer(const std::string& lua_class_name);
	virtual ~ScriptLayer();

	virtual void OnUpdate(Li::Duration::us dt) override;
	virtual void OnEvent(SDL_Event* event) override;
#ifndef LI_DIST
	virtual void OnImGuiRender() override;
#endif

private:
	void InitLuaClass(const std::string& lua_class_name);
	void InitSystems(const std::string& lua_class_name);

	ECS m_ECS;
	sol::table m_Layer;
};
