#pragma once

#include <stdint.h>

#include "readerwriterqueue/readerwriterqueue.h"
#include "entt/entt.hpp"

namespace cp
{
	struct sync_manager
	{
		uint64_t count = 0;
	};

	struct sync_tracker
	{
		std::unordered_map<uint64_t, entt::entity> map;
	};

	struct sync
	{
		uint64_t sync_id = 0;
	};
}

enum class SyncType
{
	None = 0,

	CreateEntity,
	RemoveEntity,

	AddComponent,
	RemoveComponent
};

struct SyncEvent
{
	SyncType type = SyncType::None;
	uint64_t sync_id = 0;
	
	// [AddComponent] Pointer to component of type (type).
	void* data = nullptr;

	// [AddComponent][RemoveComponent] Set to entt::type_info<(type)>::id().
	uint32_t data1 = 0; 
	uint32_t data2 = 0;


	static inline SyncEvent CreateEntity(uint64_t sync_id)
	{
		SyncEvent event;
		event.sync_id = sync_id;
		event.type = SyncType::CreateEntity;
		return event;
	}

	static inline SyncEvent RemoveEntity(uint64_t sync_id)
	{
		SyncEvent event;
		event.sync_id = sync_id;
		event.type = SyncType::RemoveEntity;
		return event;
	}

	template<typename component_t>
	static inline SyncEvent AddComponent(uint64_t sync_id, component_t* component)
	{
		SyncEvent event;
		event.sync_id = sync_id;
		event.type = SyncType::AddComponent;

		event.data = (void*)component;
		event.data1 = entt::type_info<component_t>::id();

		return event;
	}

	template<typename component_t>
	static inline SyncEvent RemoveComponent(uint64_t sync_id)
	{
		SyncEvent event;
		event.sync_id = sync_id;
		event.type = SyncType::RemoveComponent;

		event.data1 = entt::type_info<component_t>::id();

		return event;
	}
};
using SyncEventQueue = moodycamel::ReaderWriterQueue<SyncEvent>;