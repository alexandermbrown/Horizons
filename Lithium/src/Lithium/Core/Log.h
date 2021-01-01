#pragma once

#include "Lithium/Core/Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Li
{
	namespace Log
	{
		void InitLog();

		Ref<spdlog::logger>& GetCoreLogger();
		Ref<spdlog::logger>& GetClientLogger();
	};
}

// Core log macros
#define LI_CORE_TRACE(...)    ::Li::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LI_CORE_DTRACE(...)   ::Li::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define LI_CORE_INFO(...)     ::Li::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LI_CORE_WARN(...)     ::Li::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LI_CORE_ERROR(...)    ::Li::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LI_CORE_CRITICAL(...) ::Li::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define LI_TRACE(...)         ::Li::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LI_DTRACE(...)        ::Li::Log::GetClientLogger()->debug(__VA_ARGS__)
#define LI_INFO(...)          ::Li::Log::GetClientLogger()->info(__VA_ARGS__)
#define LI_WARN(...)          ::Li::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LI_ERROR(...)         ::Li::Log::GetClientLogger()->error(__VA_ARGS__)
#define LI_CRITICAL(...)      ::Li::Log::GetClientLogger()->critical(__VA_ARGS__)
