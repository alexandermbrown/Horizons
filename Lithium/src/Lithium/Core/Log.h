#pragma once

#include "Lithium/Core/Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Li
{
	namespace Log
	{
		template<typename... Args>
		inline void Trace(Args&&... args)
		{
			GetClientLogger()->trace(std::forward<Args>(args)...);
		}

		template<typename... Args>
		inline void Debug(Args&&... args)
		{
			GetClientLogger()->debug(std::forward<Args>(args)...);
		}

		template<typename... Args>
		inline void Info(Args&&... args)
		{
			GetClientLogger()->info(std::forward<Args>(args)...);
		}

		template<typename... Args>
		inline void Warn(Args&&... args)
		{
			GetClientLogger()->warn(std::forward<Args>(args)...);
		}

		template<typename... Args>
		inline void Error(Args&&... args)
		{
			GetClientLogger()->error(std::forward<Args>(args)...);
		}

		template<typename... Args>
		inline void Fatal(Args&&... args)
		{
			GetClientLogger()->critical(std::forward<Args>(args)...);
		}

		template<typename... Args>
		inline void CoreTrace(Args&&... args)
		{
			GetCoreLogger()->trace(std::forward<Args>(args)...);
		}

		template<typename... Args>
		inline void CoreDebug(Args&&... args)
		{
			GetCoreLogger()->debug(std::forward<Args>(args)...);
		}

		template<typename... Args>
		inline void CoreInfo(Args&&... args)
		{
			GetCoreLogger()->info(std::forward<Args>(args)...);
		}

		template<typename... Args>
		inline void CoreWarn(Args&&... args)
		{
			GetCoreLogger()->warn(std::forward<Args>(args)...);
		}

		template<typename... Args>
		inline void CoreError(Args&&... args)
		{
			GetCoreLogger()->error(std::forward<Args>(args)...);
		}

		template<typename... Args>
		inline void CoreFatal(Args&&... args)
		{
			GetCoreLogger()->critical(std::forward<Args>(args)...);
		}

		void InitLog();

		spdlog::logger* GetCoreLogger();
		spdlog::logger* GetClientLogger();
	};
}
