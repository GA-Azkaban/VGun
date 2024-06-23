#include "RoundTimer.h"
#include <thread>
#include <chrono>

RoundTimer::RoundTimer(int time)
	: _time(time)
{

}

void RoundTimer::Start()
{
	if (_time == 0) _isTimerEnd = true;

	//_start_time = std::chrono::steady_clock::now();

	//	auto current_time = std::chrono::steady_clock::now();
	//	std::chrono::duration<double> elapsed_seconds = current_time - start_time;

	//	if (elapsed_seconds.count() >= 1.0) {
	//		std::cout << start << std::endl;
	//		--start;
	//		start_time = current_time; // Reset the start time
	//	}
}

void RoundTimer::Update()
{

}
