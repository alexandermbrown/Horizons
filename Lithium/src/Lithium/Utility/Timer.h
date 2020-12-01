#pragma once

#include "Time.h"
#include <stdint.h>

namespace li
{
	class Timer
	{
	public:
		Timer(duration::us delay, bool completionValue = false, bool repeat = false);

		bool Update(duration::us dt);

		inline duration::us GetDelay() const
		{
			return m_Delay;
		}

		inline duration::us GetElapsed() const
		{
			return m_Elapsed;
		}

		inline float GetFraction() const
		{
			return (float)m_Elapsed.count() / (float)m_Delay.count();
		}

		inline int GetRepeatCount() const
		{
			return m_RepeatCount;
		}

		inline void SetDelay(duration::us delay)
		{
			m_Delay = delay;
		}

	private:
		duration::us m_Delay;
		duration::us m_Elapsed;

		bool m_Repeat;
		int m_RepeatCount;

		bool m_Completed;
		bool m_CompletionValue;
	};
}