//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

// CpuTimer, WallClockTimer
#include <System/Timer.hpp>

//-----------------------------------------------------------------------------
// System Includes
//-----------------------------------------------------------------------------

// log
#include <cmath>
// cout, endl
#include <iostream>

//-----------------------------------------------------------------------------
// Declarations
//-----------------------------------------------------------------------------
int main()
{
	mage::WallClockTimer wall_clock_timer;
	mage::CpuTimer cpu_timer;

	wall_clock_timer.Start();
	cpu_timer.Start();

	//  Perform some computation.
	double sum = 0.0;
	#pragma omp parallel for schedule(static, 1) reduction(+ : sum)
	for (std::int64_t i = 0; i < 10000000000; ++i)
	{
		sum += std::log(static_cast< double >(i));
	}

	using mage::TimeIntervalSeconds;

	const auto wall_clock_time 
		= wall_clock_timer.DeltaTime< TimeIntervalSeconds >();
	const auto cpu_time 
		= cpu_timer.DeltaTime< TimeIntervalSeconds >();

	std::cout << "System Time = " << wall_clock_time.count() << std::endl;
	std::cout << "Core   Time = " << cpu_time.count()        << std::endl;

	return 0;
}
