#pragma once

#include "glm/glm.hpp"
#include "AudioBuffer.h"

namespace li
{
	class AudioSource
	{
	public:
		AudioSource(float gain = 1.0f, float pitch = 1.0f, glm::vec3 position = { 0.0f, 0.0f, 0.0f }, glm::vec3 velocity = { 0.0f, 0.0f, 0.0f },  bool looping = false);
		virtual ~AudioSource();

		void SetAudio(const Ref<AudioBuffer>& audio);
		void Play();
		void Stop();

	private:
		uint32_t m_SourceID;
	};
}