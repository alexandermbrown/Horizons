#pragma once

#include <chrono>

namespace Li
{
	namespace Duration
	{
		using ns = std::chrono::nanoseconds;
		using us = std::chrono::microseconds;
		using ms = std::chrono::milliseconds;
		using sec = std::chrono::seconds;
		using fsec = std::chrono::duration<float>;

		template<typename ToDuration, typename Rep, typename Period>
		constexpr ToDuration Cast(const std::chrono::duration<Rep, Period>& d)
		{
			return std::chrono::duration_cast<ToDuration>(d);
		}
	}
}
