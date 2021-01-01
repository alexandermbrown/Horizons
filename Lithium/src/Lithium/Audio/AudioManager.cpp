#include "lipch.h"
#include "AudioManager.h"

#define AL_LIBTYPE_STATIC
#include "AL/alc.h"

namespace Li
{
	struct AudioData
	{
		AudioData(ALCdevice* device, ALCcontext* context)
			: Device(device), Context(context) {}
		ALCdevice* Device;
		ALCcontext* Context;
	};

	Unique<AudioData> s_Data = nullptr;

	bool AudioManager::Init(const char* device_name)
	{
		// TODO: Enumerate through devices and allow user to change the device.
		ALCdevice* device = alcOpenDevice(device_name);
		if (device == nullptr)
		{
			LI_CORE_ERROR("Failed to open audio device.");
			return false;
		}
		ALCcontext* context = alcCreateContext(device, nullptr);
		if (context == nullptr)
		{
			LI_CORE_ERROR("Failed to create audio context.");
			alcCloseDevice(device);
			return false;
		}
		if (alcMakeContextCurrent(context) == false)
		{
			LI_CORE_ERROR("Failed to make audio context current!");
			alcDestroyContext(context);
			alcCloseDevice(device);
			return false;
		}

		s_Data = MakeUnique<AudioData>(device, context);
		return true;
	}

	void AudioManager::Shutdown()
	{
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(s_Data->Context);
		alcCloseDevice(s_Data->Device);
		s_Data.reset();
	}
}
