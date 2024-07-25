#include "HitEffect.h"
#include "GameManager.h"
#include "PlayerInfo.h"

HitEffect::HitEffect()
{
	_curve.AddKey(0, 1, [](float t) { return t * t - 2.0f * t + 1.0f; });

	_hitEffectTimer.isRepeat = false;
	_hitEffectTimer.duration = 0.5f;
	_hitEffectTimer.onUpdate = [&](float progress)
		{
			float value = _curve.Evaluate(progress);
			_hitEffectImage->SetColor({ value, value, value, 1.0f });
		};
	_hitEffectTimer.onExpiration = [&]()
		{
			_hitEffectImage->SetActive(false);
		};
}

void HitEffect::Start()
{
	_hitEffectImage = GetGameObject()->AddComponent<HDData::ImageUI>();
	_hitEffectImage->GetTransform()->SetPosition(API::GetScreenWidth() / 2.0f, API::GetScreenHeight() / 2.0f, 0);
	_hitEffectImage->SetImage("hitEffect.png");
	_hitEffectImage->ChangeScale(API::GetScreenWidth() / 1920.0f, API::GetScreenHeight() / 1080.0f);
	_hitEffectImage->SetActive(false);
	_hitEffectImage->SetIsIgnoreFocus(true);

	GameManager::Instance()->GetMyInfo()->SetHitEffectObj(this);
}

void HitEffect::Update()
{
	_hitEffectTimer.Update();
}

void HitEffect::SetEffectOn()
{
	_hitEffectImage->SetActive(true);
	_hitEffectTimer.Start();
}
