//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include "system\windows.hpp"
#include "system\parallel.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------
namespace mage {

	size_t NumberOfSystemCores() {
		// Structure containing information about the current computer system. 
		SYSTEM_INFO system_info;
		// Retrieve information about the current system.
		GetSystemInfo(&system_info);
		// dwNumberOfProcessors:	The number of logical processors in the current group.
		return system_info.dwNumberOfProcessors;
	}
}