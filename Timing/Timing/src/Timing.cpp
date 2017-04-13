#include <math.h>
#include <iostream>

#include "timer\timer.hpp"
#include "timer\cpu_timer.hpp"

using namespace mage;

int main() {

	Timer system_timer;
	CPUTimer core_timer;

	system_timer.Start();
	core_timer.Start();

	//  Perform some computation.
	double sum = 0;
#pragma omp parallel for schedule(static, 1) reduction(+ : sum)
	for (int64_t i = 1; i <= 10000000000; ++i) {
		sum += log(static_cast< double >(i));
	}

	const double system_delta_time = system_timer.GetSystemDeltaTime();
	const double core_delta_time   = core_timer.GetCoreDeltaTimePerCore();

	std::cout << "System Time = " << system_delta_time << std::endl;
	std::cout << "Core Time  = "  << core_delta_time   << std::endl;
}