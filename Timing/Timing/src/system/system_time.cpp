//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include "system\system_time.hpp"
#include "system\windows.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------
namespace mage {

	//-------------------------------------------------------------------------
	// File Time
	//-------------------------------------------------------------------------
	namespace {

		/**
		 Converts the given file time to an @c U64 (in 100 ns).

		 @param[in]		ftime
						A reference to the file time.
		 @return		A @c U64 (in 100 ns) representing the given file time.
		 */
		[[nodiscard]]
		inline U64 ConvertTimestamp(const FILETIME& ftime) noexcept {
			return static_cast< U64 >(ftime.dwLowDateTime)
				 | static_cast< U64 >(ftime.dwHighDateTime) << 32u;
		}
	}

	//-------------------------------------------------------------------------
	// System Time
	//-------------------------------------------------------------------------
	#pragma region

	namespace {

		/**
		 Returns the current system timestamp (in 100 ns).

		 @return		The current system timestamp (in 100 ns).
		 */
		[[nodiscard]]
		U64 GetSystemTimestamp() noexcept {
			// Retrieves the current system date and time.
			// The information is in Coordinated Universal Time (UTC) format.
			FILETIME ftime;
			GetSystemTimeAsFileTime(&ftime);

			FILETIME local_ftime;
			return (FALSE == FileTimeToLocalFileTime(&ftime, &local_ftime)) 
				   ? 0ull : ConvertTimestamp(local_ftime);
		}
	}

	[[nodiscard]]
	typename const SystemClock::time_point SystemClock::now() noexcept {
		return time_point(duration(GetSystemTimestamp()));
	}

	#pragma endregion

	//-------------------------------------------------------------------------
	// Core Time
	//-------------------------------------------------------------------------
	#pragma region

	namespace {
		
		[[nodiscard]]
		U16 NumberOfSystemCores() noexcept {
			// Structure containing information about the current computer system. 
			SYSTEM_INFO system_info = {};
			// Retrieve information about the current system.
			GetSystemInfo(&system_info);
			// dwNumberOfProcessors:	The number of logical processors in the 
			//                          current group.
			return static_cast< U16 >(system_info.dwNumberOfProcessors);
		}

		/**
		 The number of system cores.
		 */
		const U16 g_nb_system_cores = NumberOfSystemCores();

		/**
		 Returns the current core timestamps (in 100 ns).

		 @return		A pair containing the current kernel and user mode 
						timestamp of the calling process.
		 @note			If the retrieval fails, both the kernel and user mode 
						timestamp are zero. To get extended error information, 
						call @c GetLastError.
		 */
		[[nodiscard]]
		const std::pair< U64, U64 > GetCoreTimestamps() noexcept {
			FILETIME ftime;
			FILETIME kernel_mode_ftime;
			FILETIME user_mode_ftime;
			// Retrieves timing information for the specified process.
			// 1. A handle to the process whose timing information is sought.
			// 2. A pointer to a FILETIME structure that receives the creation  
			//    time of the process.
			// 3. A pointer to a FILETIME structure that receives the exit time 
			//    of the process.
			// 4. A pointer to a FILETIME structure that receives the amount of  
			//    time that the process has executed in kernel mode.
			// 5. A pointer to a FILETIME structure that receives the amount of  
			//    time that the process has executed in user mode.
			const BOOL result = GetProcessTimes(GetCurrentProcess(),
												&ftime, 
												&ftime, 
												&kernel_mode_ftime, 
												&user_mode_ftime);
			if (FALSE == result) {
				return {};
			}
			
			return { 
				ConvertTimestamp(kernel_mode_ftime),  
				ConvertTimestamp(user_mode_ftime) 
			};
		}

		/**
		 Returns the current core timestamp (in 100 ns).
	 
		 @return		The current core timestamp of the calling process.
		 */
		[[nodiscard]]
		inline U64 GetCoreTimestamp() noexcept {
			const auto timestamps = GetCoreTimestamps();
			return timestamps.first + timestamps.second;
		}

		/**
		 Returns the current kernel mode core timestamp (in 100 ns).
	 
		 @return		The current kernel mode core timestamp of the calling 
						process.
		 */
		[[nodiscard]]
		inline U64 GetKernelModeCoreTimestamp() noexcept {
			const auto timestamps = GetCoreTimestamps();
			return timestamps.first;
		}

		/**
		 Returns the current user mode core timestamp (in 100 ns).
	 
		 @return		The current user mode core timestamp of the calling 
						process.
		 */
		[[nodiscard]]
		inline U64 GetUserModeCoreTimestamp() noexcept {
			const auto timestamps = GetCoreTimestamps();
			return timestamps.second;
		}

		/**
		 Returns the current core timestamp per system core (in 100 ns).
	 
		 @return		The current core timestamp of the calling process per 
						system core.
		 */
		[[nodiscard]]
		inline U64 GetCoreTimestampPerCore() noexcept {
			return GetCoreTimestamp() / g_nb_system_cores;
		}

		/**
		 Returns the current kernel mode core timestamp per system core (in 100 ns).
	 
		 @return		The current kernel mode core timestamp of the calling 
						process per system core.
		 */
		[[nodiscard]]
		inline U64 GetKernelModeCoreTimestampPerCore() noexcept {
			return GetKernelModeCoreTimestamp() / g_nb_system_cores;
		}

		/**
		 Returns the current user mode core timestamp per system core (in 100 ns).
	 
		 @return		The current user mode core timestamp of the calling 
						process per system core.
		 */
		[[nodiscard]]
		inline U64 GetUserModeCoreTimestampPerCore() noexcept {
			return GetUserModeCoreTimestamp() / g_nb_system_cores;
		}
	}

	[[nodiscard]]
	typename const CoreClock::time_point CoreClock::now() noexcept {
		return time_point(duration(GetCoreTimestamp()));
	}

	[[nodiscard]]
	typename const KernelModeCoreClock::time_point
		KernelModeCoreClock::now() noexcept {

		return time_point(duration(GetKernelModeCoreTimestamp()));
	}

	[[nodiscard]]
	typename const UserModeCoreClock::time_point
		UserModeCoreClock::now() noexcept {

		return time_point(duration(GetUserModeCoreTimestamp()));
	}

	[[nodiscard]]
	typename const CoreClockPerCore::time_point
		CoreClockPerCore::now() noexcept {

		return time_point(duration(GetCoreTimestampPerCore()));
	}

	[[nodiscard]]
	typename const KernelModeCoreClockPerCore::time_point
		KernelModeCoreClockPerCore::now() noexcept {

		return time_point(duration(GetKernelModeCoreTimestampPerCore()));
	}

	[[nodiscard]]
	typename const UserModeCoreClockPerCore::time_point
		UserModeCoreClockPerCore::now() noexcept {

		return time_point(duration(GetUserModeCoreTimestampPerCore()));
	}

	#pragma endregion
}