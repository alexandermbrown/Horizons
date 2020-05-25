#pragma once

#include <stdint.h>

namespace li
{
	template<typename DeltaType = float>
	class Timer
	{
	public:

		inline Timer(DeltaType delay, bool completionValue = false, bool repeat = false) :
			m_Delay(delay),
			m_CompletionValue(completionValue),
			m_Repeat(repeat),
			m_Elapsed(),
			m_RepeatCount(0U),
			m_Completed(false) {}

		bool Update(DeltaType dt)
		{
			if (m_Completed)
				return m_CompletionValue;

			m_Elapsed += dt;

			if (m_Elapsed >= m_Delay) {
				if (!m_Repeat)
					m_Completed = true;
				else {
					m_Elapsed = 0;
					m_RepeatCount++;
				}
				return true;
			}
			return false;
		}

		inline DeltaType GetDelay()
		{
			return m_Delay;
		}

		inline DeltaType GetElapsed()
		{
			return m_Elapsed;
		}

		inline DeltaType GetFraction()
		{
			return m_Elapsed / m_Delay;
		}

		inline uint32_t GetRepeatCount()
		{
			return m_RepeatCount;
		}

		inline void SetDelay(DeltaType delay)
		{
			m_Delay = delay;
		}

	private:

		DeltaType m_Delay;
		DeltaType m_Elapsed;

		bool m_Repeat;
		uint32_t m_RepeatCount;

		bool m_Completed;
		bool m_CompletionValue;
	};
}