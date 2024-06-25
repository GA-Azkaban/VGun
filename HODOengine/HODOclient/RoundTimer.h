#pragma once
#include "../HODOengine/HODO_API.h"
#include <thread>
#include <chrono>

class RoundTimer : public HDData::Script
{
public:
	RoundTimer(int time);

public:
	void Start() override;
	void Update() override;

private:
	HDData::TextUI* _txt;
	
private:
	std::chrono::time_point<std::chrono::steady_clock> _start_time;
	double _duration;
	bool _isRunning;
};

