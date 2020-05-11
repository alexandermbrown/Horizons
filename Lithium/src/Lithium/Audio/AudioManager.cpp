#include "lipch.h"
#include "AudioManager.h"

#define AL_LIBTYPE_STATIC
#include "AL/alc.h"

namespace li
{
	Scope<AudioManager> AudioManager::s_Instance = CreateScope<AudioManager>();

	AudioManager::AudioManager()
		: m_Device(NULL), m_Context(NULL)
	{
	}

	void AudioManager::InitImpl()
	{
		// TODO: Enumerate through devices and allow user to change the device.
		m_Device = alcOpenDevice(nullptr);
		// TODO: Handle error without crashing.
		LI_CORE_ASSERT(m_Device, "Failed to create audio device!");

		m_Context = alcCreateContext(static_cast<ALCdevice*>(m_Device), NULL);
		LI_CORE_ASSERT(m_Context, "Failed to create audio context!");

		if (!alcMakeContextCurrent(static_cast<ALCcontext*>(m_Context))) {
			LI_CORE_ERROR("Failed to make audio context current!");
		}
	}

	void AudioManager::ShutdownImpl()
	{
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(static_cast<ALCcontext*>(m_Context));
		m_Context = nullptr;

		alcCloseDevice(static_cast<ALCdevice*>(m_Device));
		m_Device = nullptr;
	}
}
