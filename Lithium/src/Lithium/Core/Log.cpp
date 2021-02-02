#include "lipch.h"
#include "Log.h"

#include "Lithium/Core/Memory.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Li
{
	Ref<spdlog::logger> s_CoreLogger;
	Ref<spdlog::logger> s_ClientLogger;

	void Log::InitLog()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("LITHIUM");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

	spdlog::logger* Log::GetCoreLogger()
	{
		return s_CoreLogger.get();
	}

	spdlog::logger* Log::GetClientLogger()
	{
		return s_ClientLogger.get();
	}
}
