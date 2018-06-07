#include <math.h>
#include <iostream>

#include "system\timer.hpp"

int main() {

	mage::WallClockTimer wall_clock_timer;
	mage::CPUTimer cpu_timer;

	wall_clock_timer.Start();
	cpu_timer.Start();

	//  Perform some computation.
	double sum = 0;
	#pragma omp parallel for schedule(static, 1) reduction(+ : sum)
	for (int64_t i = 1; i <= 10000000000; ++i) {
		sum += log(static_cast< double >(i));
	}

	const auto wall_clock_time = wall_clock_timer.GetDeltaTime();
	const auto cpu_time        = cpu_timer.GetDeltaTime();

	std::cout << "System Time = " << wall_clock_time.count() << std::endl;
	std::cout << "Core Time  = "  << cpu_time.count()        << std::endl;
}