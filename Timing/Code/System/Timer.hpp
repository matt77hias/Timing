#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

// CoreClockPerCore
#include <System/SystemTime.hpp>
// F64
#include <Type/ScalarTypes.hpp>

//-----------------------------------------------------------------------------
// External Includes
//-----------------------------------------------------------------------------

// duration, duration_cast, high_resolution_clock
#include <chrono>
// pair
#include <utility>

//-----------------------------------------------------------------------------
// Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage
{
	//-------------------------------------------------------------------------
	// Time
	//-------------------------------------------------------------------------

	/**
	 A time stamp type expressed in seconds.
	 */
	using TimeStampSeconds = std::chrono::duration< F64 >;

	/**
	 A time interval type expressed in seconds.
	 */
	using TimeIntervalSeconds = std::chrono::duration< F64 >;

	//-------------------------------------------------------------------------
	// Timer
	//-------------------------------------------------------------------------

	/**
	 A class of timers.

	 @tparam		ClockT
					The clock type.
	 */
	template< typename ClockT >
	class Timer
	{

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a timer.
		 */
		Timer() noexcept = default;

		/**
		 Constructs a timer from the given timer.

		 @param[in]		timer
						A reference to the timer to copy.
		 */
		Timer(const Timer& timer) noexcept = default;

		/**
		 Constructs a timer by moving the given timer.

		 @param[in]		timer
						A reference to the timer to move.
		 */
		Timer(Timer&& timer) noexcept = default;

		/**
		 Destructs this timer.
		 */
		~Timer() = default;

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		/**
		 Copies the given timer to this timer.

		 @param[in]		timer
						A reference to the timer to copy.
		 @return		A reference to the copy of the given timer (i.e. this
						timer).
		 */
		Timer& operator=(const Timer& timer) noexcept = default;

		/**
		 Moves the given timer to this timer.

		 @param[in]		timer
						A reference to the timer to move.
		 @return		A reference to the moved timer (i.e. this timer).
		 */
		Timer& operator=(Timer&& timer) noexcept = default;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Starts this timer.
		 */
		void Start() noexcept;

		/**
		 Stops this timer.
		 */
		void Stop() noexcept;

		/**
		 Restarts this timer.
		 */
		void Restart() noexcept;

		/**
		 Resumes this timer.
		 */
		void Resume() noexcept;

		//---------------------------------------------------------------------
		// Member Methods: (Total) Delta Time
		//---------------------------------------------------------------------

		/**
		 Returns the delta time (in seconds) of this timer.

		 @tparam		TimeIntervalT
						The time interval type.
		 @return		The delta time (in seconds) of this timer.
		 */
		template< typename TimeIntervalT >
		TimeIntervalT DeltaTime() noexcept;

		/**
		 Returns the total delta time (in seconds) of this timer.

		 @tparam		TimeIntervalT
						The time interval type.
		 @return		The total delta time (in seconds) of this timer.
		 */
		template< typename TimeIntervalT >
		TimeIntervalT TotalDeltaTime() noexcept;

		/**
		 Returns the delta and total delta time (in seconds) of this timer.

		 @tparam		TimeIntervalT
						The time interval type.
		 @return		A pair containing the the delta and total delta time
						(in seconds) of this timer.
		 */
		template< typename TimeIntervalT >
		std::pair< TimeIntervalT, TimeIntervalT > Time() noexcept;

	private:

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		/**
		 Resets the delta time, total delta time and last timestamp of this
		 timer.
		 */
		void ResetDeltaTime() noexcept;

		/**
		 Updates the delta time, total delta time and last timestamp of this
		 timer.
		 */
		void UpdateDeltaTime() noexcept;

		//---------------------------------------------------------------------
		// Class Member Types
		//---------------------------------------------------------------------

		/**
		 The time stamp type representing the time points of timers.
		 */
		using TimeStamp = typename ClockT::time_point;

		/**
		 The time interval type representing the interval between time points
		 of timers.
		 */
		using TimeInterval = typename ClockT::duration;

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		/**
		 The clock of this timer.
		 */
		ClockT m_clock = {};

		/**
		 The last timestamp of this timer.
		 */
		TimeStamp m_last_timestamp = TimeStamp::min();

		/**
		 The delta time of this timer.
		 */
		TimeInterval m_delta_time = TimeInterval::zero();

		/**
		 The total delta time of this timer.
		 */
		TimeInterval m_total_delta_time = TimeInterval::zero();

		/**
		 Flag indicating whether this timer is running.
		 */
		bool m_running = false;
	};

	//-------------------------------------------------------------------------
	// Type Declarations and Definitions
	//-------------------------------------------------------------------------

	/**
	 A class of wall clock timers.
	 */
	using WallClockTimer = Timer< std::chrono::high_resolution_clock >;

	/**
	 A class of CPU (i.e. core clock per core) timers.
	 */
	using CpuTimer = Timer< CoreClockPerCore >;
}

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <System/Timer.tpp>
