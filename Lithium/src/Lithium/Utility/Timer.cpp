#include "lipch.h"
#include "Timer.h"

namespace Li
{
	Timer::Timer(Duration::us delay, bool completionValue, bool repeat) :
		m_Delay(delay),
		m_CompletionValue(completionValue),
		m_Repeat(repeat),
		m_Elapsed(),
		m_RepeatCount(0U),
		m_Completed(false)
	{
	}

	bool Timer::Update(Duration::us dt)
	{
		if (m_Completed)
			return m_CompletionValue;

		m_Elapsed += dt;

		if (m_Elapsed >= m_Delay) {
			if (!m_Repeat)
				m_Completed = true;
			else {
				m_Elapsed = Duration::us(0);
				m_RepeatCount++;
			}
			return true;
		}
		return false;
	}
}
