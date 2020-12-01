#include "lipch.h"
#include "Timer.h"

namespace li
{
	Timer::Timer(duration::us delay, bool completionValue, bool repeat) :
		m_Delay(delay),
		m_CompletionValue(completionValue),
		m_Repeat(repeat),
		m_Elapsed(),
		m_RepeatCount(0U),
		m_Completed(false)
	{
	}

	bool Timer::Update(duration::us dt)
	{
		if (m_Completed)
			return m_CompletionValue;

		m_Elapsed += dt;

		if (m_Elapsed >= m_Delay) {
			if (!m_Repeat)
				m_Completed = true;
			else {
				m_Elapsed = duration::us(0);
				m_RepeatCount++;
			}
			return true;
		}
		return false;
	}
}
