#include "RoundTimer.h"
#include <string>

RoundTimer::RoundTimer(int time)
	: _duration(time),
	_start_time(std::chrono::steady_clock::now()),
	_isRunning(false)
{

}

void RoundTimer::Start()
{
	_txt = GetGameObject()->GetComponent<HDData::TextUI>();
	_txt->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	_txt->GetTransform()->SetPosition(2400.0f, 100.0f, 0.0f);
	_isRunning = true;
}

void RoundTimer::Update()
{
	if (_isRunning) 
	{
		auto currentTime = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsedTime = currentTime - _start_time;
		_txt->SetText(std::to_string(static_cast<int>(_duration - elapsedTime.count())));
		if (elapsedTime.count() >= _duration) 
		{
			_isRunning = false;
		}
	}
}
