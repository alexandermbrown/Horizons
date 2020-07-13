#include "pch.h"
#include "SyncInitSystem.h"

#include "Horizons/Gameplay/Sync/Sync.h"

void SyncInitSystem::Init(entt::registry& registry)
{
	registry.set<cp::sync_manager>(0ull);
}
