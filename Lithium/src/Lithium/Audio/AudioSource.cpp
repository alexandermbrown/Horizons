#include "lipch.h"
#include "AudioSource.h"

#include "AudioCore.h"

#define AL_LIBTYPE_STATIC
#include "AL/al.h"

namespace li
{
	AudioSource::AudioSource(float gain, float pitch, glm::vec3 position, glm::vec3 velocity, bool looping)
	{
		ALCall( alGenSources(1, &m_SourceID) );
		ALCall( alSourcef(m_SourceID, AL_GAIN, gain) );
		ALCall( alSourcef(m_SourceID, AL_PITCH, pitch) );
		ALCall( alSource3f(m_SourceID, AL_POSITION, position.x, position.y, position.z) );
		ALCall( alSource3f(m_SourceID, AL_VELOCITY, velocity.x, velocity.y, velocity.z) );
		ALCall( alSourcei(m_SourceID, AL_LOOPING, looping ? AL_TRUE : AL_FALSE) );
	}

	AudioSource::~AudioSource()
	{
		ALCall( alDeleteSources(1, &m_SourceID) );
	}

	void AudioSource::SetAudio(const Ref<Audio>& audio)
	{
		ALCall( alSourcei(m_SourceID, AL_BUFFER, audio->GetBufferID()) );
	}

	void AudioSource::Play()
	{
		ALCall( alSourcePlay(m_SourceID) );
	}
}


