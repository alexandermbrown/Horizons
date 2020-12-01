#pragma once

#include "Lithium/Core/Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace li
{
	namespace Log
	{
		void InitLog();

		Ref<spdlog::logger>& GetCoreLogger();
		Ref<spdlog::logger>& GetClientLogger();
	};
}

// Core log macros
#define LI_CORE_TRACE(...)    ::li::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LI_CORE_DTRACE(...)   ::li::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define LI_CORE_INFO(...)     ::li::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LI_CORE_WARN(...)     ::li::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LI_CORE_ERROR(...)    ::li::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LI_CORE_CRITICAL(...) ::li::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define LI_TRACE(...)         ::li::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LI_DTRACE(...)        ::li::Log::GetClientLogger()->debug(__VA_ARGS__)
#define LI_INFO(...)          ::li::Log::GetClientLogger()->info(__VA_ARGS__)
#define LI_WARN(...)          ::li::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LI_ERROR(...)         ::li::Log::GetClientLogger()->error(__VA_ARGS__)
#define LI_CRITICAL(...)      ::li::Log::GetClientLogger()->critical(__VA_ARGS__)
