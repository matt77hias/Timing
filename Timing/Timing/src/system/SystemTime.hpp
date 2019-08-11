#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

// U16, U64
#include <Type/ScalarTypes.hpp>

//-----------------------------------------------------------------------------
// External Includes
//-----------------------------------------------------------------------------

// duration, time_point
#include <chrono>
// ratio
#include <ratio>

//-----------------------------------------------------------------------------
// Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage
{
	//-------------------------------------------------------------------------
	// System Time
	//-------------------------------------------------------------------------

	struct SystemClock
	{
		using rep        = U64;
		using period     = std::ratio< 1, 10'000'000 >;
		using duration   = std::chrono::duration< rep, period >;
		using time_point = std::chrono::time_point< SystemClock >;

		static constexpr bool is_steady = true;

		[[nodiscard]]
		static time_point now() noexcept;
	};

	//-------------------------------------------------------------------------
	// Core Time
	//-------------------------------------------------------------------------

	struct CoreClock
	{
		using rep        = U64;
		using period     = std::ratio< 1, 10'000'000 >;
		using duration   = std::chrono::duration< rep, period >;
		using time_point = std::chrono::time_point< CoreClock >;

		static constexpr bool is_steady = true;

		[[nodiscard]]
		static time_point now() noexcept;
	};

	struct KernelModeCoreClock
	{
		using rep        = U64;
		using period     = std::ratio< 1, 10'000'000 >;
		using duration   = std::chrono::duration< rep, period >;
		using time_point = std::chrono::time_point< KernelModeCoreClock >;

		static constexpr bool is_steady = true;

		[[nodiscard]]
		static time_point now() noexcept;
	};

	struct UserModeCoreClock
	{
		using rep        = U64;
		using period     = std::ratio< 1, 10'000'000 >;
		using duration   = std::chrono::duration< rep, period >;
		using time_point = std::chrono::time_point< UserModeCoreClock >;

		static constexpr bool is_steady = true;

		[[nodiscard]]
		static time_point now() noexcept;
	};

	struct CoreClockPerCore
	{
		using rep        = U64;
		using period     = std::ratio< 1, 10'000'000 >;
		using duration   = std::chrono::duration< rep, period >;
		using time_point = std::chrono::time_point< CoreClockPerCore >;

		static constexpr bool is_steady = true;

		[[nodiscard]]
		static time_point now() noexcept;
	};

	struct KernelModeCoreClockPerCore
	{
		using rep        = U64;
		using period     = std::ratio< 1, 10'000'000 >;
		using duration   = std::chrono::duration< rep, period >;
		using time_point = std::chrono::time_point< KernelModeCoreClockPerCore >;

		static constexpr bool is_steady = true;

		[[nodiscard]]
		static time_point now() noexcept;
	};

	struct UserModeCoreClockPerCore
	{
		using rep        = U64;
		using period     = std::ratio< 1, 10'000'000 >;
		using duration   = std::chrono::duration< rep, period >;
		using time_point = std::chrono::time_point< UserModeCoreClockPerCore >;

		static constexpr bool is_steady = true;

		[[nodiscard]]
		static time_point now() noexcept;
	};
}