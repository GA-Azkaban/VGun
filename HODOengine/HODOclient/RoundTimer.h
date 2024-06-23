#pragma once
#include "../HODOengine/HODO_API.h"

class RoundTimer : public HDData::Script
{
public:
	RoundTimer(int time);

public:
	void Start() override;
	void Update() override;

private:
	int _time;
	bool _isTimerEnd;

private:
	long _start_time;
	long _end_time;
};

