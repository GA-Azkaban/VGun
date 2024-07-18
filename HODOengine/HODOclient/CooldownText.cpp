#include "CooldownText.h"

CooldownText::CooldownText()
{
	_cooldownTimer.isRepeat = false;
	_cooldownTimer.duration = 6.0f;
	_cooldownTimer.onUpdate = [&](float progress)
		{
			int remainTime = static_cast<int>(_cooldownTimer.duration - _cooldownTimer.GetElapsedTime());
			_cooldownText->SetText(std::to_string(remainTime));
		};
	_cooldownTimer.onExpiration = [&]()
		{
			_cooldownText->SetActive(false);
		};
}

void CooldownText::Start()
{
	_cooldownText = GetGameObject()->GetComponent<HDData::TextUI>();
	_cooldownText->SetActive(false);
}

void CooldownText::Update()
{
	_cooldownTimer.Update();
}

void CooldownText::SetTimerOn()
{
	_cooldownText->SetActive(true);
	_cooldownTimer.Start();
}
