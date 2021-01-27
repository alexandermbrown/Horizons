#pragma once

#include "Horizons/Core/Core.h"
#include "Horizons/Core/AppState.h"
#include "Horizons/Config/ConfigStore.h"
#include "Horizons/Config/ConfigLoader.h"
#include "Horizons/Scripting/ScriptContext.h"
#include "Horizons/Scripting/TerrainData.h"
#include "Horizons/ECS/SystemRegistry.h"

#ifndef LI_DIST
#include "Horizons/Console/Console.h"
#endif

#include "Lithium.h"

class Horizons : public Li::Application
{
public:
	Horizons(Li::RendererAPI renderer_api);
	virtual ~Horizons();

	inline ConfigStore& GetConfig() { return m_ConfigStore; }
	inline ScriptContext& GetScriptContext() { return m_ScriptContext; }
	inline TerrainData& GetTerrainData() { return m_TerrainData; }
	inline const SystemRegistry& GetSystemRegistry() const { return m_SystemRegistry; }

#ifndef LI_DIST
	inline Console& GetConsole() { return m_Console; }
#endif

private:
	void LoadConfig();
	void SaveConfig();

	ConfigLoader m_ConfigLoader;
	ConfigStore m_ConfigStore;

	ScriptContext m_ScriptContext;
	TerrainData m_TerrainData;
	SystemRegistry m_SystemRegistry;

#ifndef LI_DIST
	Console m_Console;
#endif
};
