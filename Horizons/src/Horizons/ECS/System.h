#pragma once

#include "Lithium.h"
#include "entt/entt.hpp"
#include "SDL.h"
#include <functional>

struct System
{
	using OnInitFn = std::function<void(entt::registry&)>;
	using OnEventFn = std::function<void(entt::registry&, SDL_Event*)>;
	using OnUpdateFn = std::function<void(entt::registry&, Li::Duration::us)>;
	using OnRenderFn = std::function<void(entt::registry&)>;
	using OnDeinitFn = std::function<void(entt::registry&)>;

	OnInitFn OnInit;
	OnEventFn OnEvent;
	OnUpdateFn OnUpdate;
	OnRenderFn OnRender;
	OnDeinitFn OnDeinit;

	inline System& SetOnInit(OnInitFn func)     { OnInit = func;   return *this; }
	inline System& SetOnEvent(OnEventFn func)   { OnEvent = func;  return *this; }
	inline System& SetOnUpdate(OnUpdateFn func) { OnUpdate = func; return *this; }
	inline System& SetOnRender(OnRenderFn func) { OnRender = func; return *this; }
	inline System& SetOnDeinit(OnDeinitFn func) { OnDeinit = func; return *this; }
};
