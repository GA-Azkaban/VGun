#include "LowHPEffect.h"
#include "GameManager.h"

LowHPEffect::LowHPEffect()
	: _gameManager(GameManager::Instance())
{
	_curve.AddKey(0.0f, 0.5f, [](float t) { return 1.8f * t + 0.2f; });
	_curve.AddKey(0.5f, 1.0f, [](float t) { return -1.8f * t + 2.0f; });

	_lowHPEffectTimer.isRepeat = true;
	_lowHPEffectTimer.duration = 1.5f;
	_lowHPEffectTimer.onUpdate = [&](float progress)
		{
			float value = _curve.Evaluate(progress);
			float effectColor = value * _effectColorThreshold;
			_lowHPEffectImage->SetColor({ effectColor, effectColor, effectColor, 1.0f });
		};
}

void LowHPEffect::Start()
{
	_lowHPEffectImage = GetGameObject()->AddComponent<HDData::ImageUI>();
	_lowHPEffectImage->GetTransform()->SetPosition(API::GetScreenWidth() / 2.0f, API::GetScreenHeight() / 2.0f, 0);
	_lowHPEffectImage->SetImage("blackout0004.png");
	_lowHPEffectImage->ChangeScale(1.334f, 1.334f);
	_lowHPEffectImage->SetActive(false);
}

void LowHPEffect::Update()
{
	_lowHPEffectTimer.Update();

	int playerHP = _gameManager->GetMyInfo()->GetPlayerCurrentHP();

	if (playerHP > (maxHP * 0.3f))
	{
		_lowHPEffectImage->SetActive(false);
		_lowHPEffectTimer.Stop();
	}
	else
	{
		_lowHPEffectImage->SetActive(true);
		if (_lowHPEffectTimer.IsActive() == false)
		{
			_lowHPEffectTimer.Start();
		}
		if (playerHP <= (maxHP * 0.3f) && playerHP > (maxHP * 0.2f))
		{
			_effectColorThreshold = 0.3f;
		}
		else if (playerHP <= (maxHP * 0.2f) && playerHP > (maxHP * 0.1f))
		{
			_effectColorThreshold = 0.66f;
		}
		else if (playerHP <= (maxHP * 0.1f) && playerHP > 0)
		{
			_effectColorThreshold = 1.0f;
		}
		else
		{
			_lowHPEffectImage->SetActive(false);
			_lowHPEffectTimer.Stop();
		}
	}
}
