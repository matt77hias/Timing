#pragma once

//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------
namespace mage
{
	template< typename ClockT >
	inline void Timer< ClockT >::Start() noexcept
	{
		if (m_running)
		{
			return;
		}

		m_running = true;
		ResetDeltaTime();
	}

	template< typename ClockT >
	inline void Timer< ClockT >::Stop() noexcept
	{
		if (!m_running)
		{
			return;
		}

		m_running = false;
		UpdateDeltaTime();
	}

	template< typename ClockT >
	inline void Timer< ClockT >::Restart() noexcept
	{
		m_running = false;
		Start();
	}

	template< typename ClockT >
	inline void Timer< ClockT >::Resume() noexcept
	{
		if (m_running)
		{
			return;
		}

		m_running = true;
		m_last_timestamp = m_clock.now();
	}

	template< typename ClockT >
	template< typename TimeIntervalT >
	inline TimeIntervalT Timer< ClockT >::DeltaTime() noexcept
	{
		if (m_running)
		{
			UpdateDeltaTime();
		}

		return std::chrono::duration_cast< TimeIntervalT >(m_delta_time);
	}

	template< typename ClockT >
	template< typename TimeIntervalT >
	inline TimeIntervalT Timer< ClockT >::TotalDeltaTime() noexcept
	{
		if (m_running)
		{
			UpdateDeltaTime();
		}

		return std::chrono::duration_cast< TimeIntervalT >(m_total_delta_time);
	}

	template< typename ClockT >
	template< typename TimeIntervalT >
	inline std::pair< TimeIntervalT, TimeIntervalT >
		Timer< ClockT >::Time() noexcept
	{
		if (m_running)
		{
			UpdateDeltaTime();
		}

		return
		{
			std::chrono::duration_cast< TimeIntervalT >(m_delta_time),
			std::chrono::duration_cast< TimeIntervalT >(m_total_delta_time)
		};
	}

	template< typename ClockT >
	inline void Timer< ClockT >::ResetDeltaTime() noexcept
	{
		// Resets the delta time of this timer.
		m_delta_time = TimeInterval::zero();
		// Resets the total delta time of this timer.
		m_total_delta_time = TimeInterval::zero();
		// Resets the last timestamp of this timer.
		m_last_timestamp = m_clock.now();
	}

	template< typename ClockT >
	inline void Timer< ClockT >::UpdateDeltaTime() noexcept
	{
		// Get the current timestamp of this timer.
		const auto current_timestamp = m_clock.now();

		// Updates the delta time of this timer.
		m_delta_time = current_timestamp - m_last_timestamp;
		// Updates the total delta time of this timer.
		m_total_delta_time += m_delta_time;
		// Updates the last timestamp of this timer.
		m_last_timestamp = current_timestamp;
	}
}
