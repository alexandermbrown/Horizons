#pragma once

#include "Lithium/Core/Core.h"

namespace li
{
	class AudioManager
	{
	public:

		AudioManager();
		virtual ~AudioManager() = default;

		inline static void Init() { s_Instance->InitImpl(); };

		inline static void Shutdown() { s_Instance->ShutdownImpl(); };

		//static void SetListener();



	private:

		void InitImpl();
		void ShutdownImpl();

		void* m_Device;
		void* m_Context;

	private:
		
		static Scope<AudioManager> s_Instance;
	};
}