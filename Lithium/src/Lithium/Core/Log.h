#pragma once

#include "Lithium/Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace li
{
	class Log
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define LI_CORE_TRACE(...)    ::li::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LI_CORE_INFO(...)     ::li::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LI_CORE_WARN(...)     ::li::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LI_CORE_ERROR(...)    ::li::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LI_CORE_CRITICAL(...) ::li::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define LI_TRACE(...)         ::li::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LI_INFO(...)          ::li::Log::GetClientLogger()->info(__VA_ARGS__)
#define LI_WARN(...)          ::li::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LI_ERROR(...)         ::li::Log::GetClientLogger()->error(__VA_ARGS__)
#define LI_CRITICAL(...)      ::li::Log::GetClientLogger()->critical(__VA_ARGS__)
