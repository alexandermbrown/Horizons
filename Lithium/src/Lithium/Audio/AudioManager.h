#pragma once

#include "Lithium/Core/Core.h"

namespace Li
{
	namespace AudioManager
	{
		// Set device_name to nullptr to automatically pick a device.
		bool Init(const char* device_name);
		void Shutdown();
	};
}