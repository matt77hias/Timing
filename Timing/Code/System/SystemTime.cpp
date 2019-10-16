//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

// Declarations
#include <System/SystemTime.hpp>
// FileTimeToLocalFileTime, GetProcessTimes, GetSystemTimeAsFileTime
#include <System/Windows.hpp>

//-----------------------------------------------------------------------------
// External Includes
//-----------------------------------------------------------------------------

// size_t
#include <cstddef>
// size
#include <iterator>
// pair
#include <utility>

//-----------------------------------------------------------------------------
// Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage
{
	//-------------------------------------------------------------------------
	// System Time
	//-------------------------------------------------------------------------
	namespace
	{
		[[nodiscard]]
		inline FILETIME GetFileTime() noexcept
		{
			// Retrieves the current system date and time.
			// The information is in Coordinated Universal Time (UTC) format.
			FILETIME file_time;
			GetSystemTimeAsFileTime(&file_time);
			return file_time;
		}

		[[nodiscard]]
		inline FILETIME GetLocalFileTime() noexcept
		{
			const auto file_time = GetFileTime();
			FILETIME local_file_time;
			if (FALSE == 
				FileTimeToLocalFileTime(&file_time, &local_file_time))
			{
				return {};
			}
			else
			{
				return local_file_time;
			}
		}
		
		/**
		 Converts the given file time to an @c U64 (in 100 ns).

		 @param[in]		file_time
						A reference to the file time.
		 @return		A @c U64 (in 100 ns) representing the given file time.
		 */
		[[nodiscard]]
		inline U64 ConvertTimestamp(const FILETIME& file_time) noexcept
		{
			return (U64(file_time.dwHighDateTime) << 32u) 
				  | U64(file_time.dwLowDateTime);
		}
	}

	[[nodiscard]]
	auto SystemClock::now() noexcept -> time_point
	{
		return time_point(duration(ConvertTimestamp(GetLocalFileTime())));
	}

	//-------------------------------------------------------------------------
	// Core Time
	//-------------------------------------------------------------------------
	namespace
	{
		/**
		 Returns the number of system cores (i.e. logical processors).

		 @return		The number of system cores (i.e. logical processors).
		 */
		[[nodiscard]]
		std::size_t NumberOfSystemCores() noexcept
		{
			SYSTEM_INFO system_info = {};
			GetSystemInfo(&system_info);
			
			// Return the number of logical processors in the current group.
			return system_info.dwNumberOfProcessors;
		}
		
		/**
		 Returns the current core timestamps (in 100 ns).

		 @return		A pair containing the current kernel and user mode
						timestamp of the calling process.
		 @note			If the retrieval fails, both the kernel and user mode
						timestamp are zero. To get extended error information,
						call @c GetLastError.
		 */
		[[nodiscard]]
		std::pair< U64, U64 > GetCoreTimestamps() noexcept
		{
			FILETIME ftime;
			FILETIME kernel_mode_ftime;
			FILETIME user_mode_ftime;
			// Retrieve timing information for the process.
			const BOOL result = GetProcessTimes(GetCurrentProcess(),
												&ftime,
												&ftime,
												&kernel_mode_ftime,
												&user_mode_ftime);
			if (FALSE == result)
			{
				return {};
			}

			return 
			{
				ConvertTimestamp(kernel_mode_ftime),
				ConvertTimestamp(user_mode_ftime)
			};
		}

		/**
		 Returns the current core timestamp (in 100 ns).

		 @return		The current core timestamp of the calling process.
		 */
		[[nodiscard]]
		inline U64 GetCoreTimestamp() noexcept
		{
			const auto timestamps = GetCoreTimestamps();
			return timestamps.first + timestamps.second;
		}

		/**
		 Returns the current kernel mode core timestamp (in 100 ns).

		 @return		The current kernel mode core timestamp of the calling
						process.
		 */
		[[nodiscard]]
		inline U64 GetKernelModeCoreTimestamp() noexcept
		{
			const auto timestamps = GetCoreTimestamps();
			return timestamps.first;
		}

		/**
		 Returns the current user mode core timestamp (in 100 ns).

		 @return		The current user mode core timestamp of the calling
						process.
		 */
		[[nodiscard]]
		inline U64 GetUserModeCoreTimestamp() noexcept
		{
			const auto timestamps = GetCoreTimestamps();
			return timestamps.second;
		}

		/**
		 Returns the current core timestamp per system core (in 100 ns).

		 @return		The current core timestamp of the calling process per
						system core.
		 */
		[[nodiscard]]
		inline U64 GetCoreTimestampPerCore() noexcept
		{
			static const auto s_system_core_count = NumberOfSystemCores();
			return GetCoreTimestamp() / s_system_core_count;
		}

		/**
		 Returns the current kernel mode core timestamp per system core (in 100 ns).

		 @return		The current kernel mode core timestamp of the calling
						process per system core.
		 */
		[[nodiscard]]
		inline U64 GetKernelModeCoreTimestampPerCore() noexcept
		{
			static const auto s_system_core_count = NumberOfSystemCores();
			return GetKernelModeCoreTimestamp() / s_system_core_count;
		}

		/**
		 Returns the current user mode core timestamp per system core (in 100 ns).

		 @return		The current user mode core timestamp of the calling
						process per system core.
		 */
		[[nodiscard]]
		inline U64 GetUserModeCoreTimestampPerCore() noexcept
		{
			static const auto s_system_core_count = NumberOfSystemCores();
			return GetUserModeCoreTimestamp() / s_system_core_count;
		}
	}

	[[nodiscard]]
	auto CoreClock::now() noexcept -> time_point
	{
		return time_point(duration(GetCoreTimestamp()));
	}

	[[nodiscard]]
	auto KernelModeCoreClock::now() noexcept -> time_point
	{
		return time_point(duration(GetKernelModeCoreTimestamp()));
	}

	[[nodiscard]]
	auto UserModeCoreClock::now() noexcept -> time_point
	{
		return time_point(duration(GetUserModeCoreTimestamp()));
	}

	[[nodiscard]]
	auto CoreClockPerCore::now() noexcept -> time_point
	{
		return time_point(duration(GetCoreTimestampPerCore()));
	}

	[[nodiscard]]
	auto KernelModeCoreClockPerCore::now() noexcept -> time_point
	{
		return time_point(duration(GetKernelModeCoreTimestampPerCore()));
	}

	[[nodiscard]]
	auto UserModeCoreClockPerCore::now() noexcept -> time_point
	{
		return time_point(duration(GetUserModeCoreTimestampPerCore()));
	}
}
