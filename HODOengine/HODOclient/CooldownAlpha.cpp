#include "CooldownAlpha.h"

CooldownAlpha::CooldownAlpha()
{
	_curve.AddKey(0, 1, [](float t) { return 1.0f - t; });

	_cooldownTimer.isRepeat = false;
	_cooldownTimer.duration = 6.0f;
	_cooldownTimer.onUpdate = [&](float progress)
		{
			float value = _curve.Evaluate(progress);
			_cooldownImage->ChangeScale(1.0f, value);
			float posY = _originalPosition.y - (1.0f - value) * 65.0f;
			GetGameObject()->GetTransform()->SetPosition(_originalPosition.x, posY, _originalPosition.z);
		};
	_cooldownTimer.onExpiration = [&]()
		{
			_cooldownImage->SetActive(false);
		};
}

void CooldownAlpha::Start()
{
	_cooldownImage = GetGameObject()->GetComponent<HDData::ImageUI>();
	_cooldownImage->SetActive(false);
	_originalPosition = GetGameObject()->GetTransform()->GetPosition();
}

void CooldownAlpha::Update()
{
	_cooldownTimer.Update();
}

void CooldownAlpha::SetTimerOn()
{
	_cooldownImage->SetActive(true);
	_cooldownTimer.Start();
	GetGameObject()->GetTransform()->SetPosition(_originalPosition);
}
