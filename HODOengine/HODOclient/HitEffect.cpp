#include "HitEffect.h"
#include "GameManager.h"
#include "PlayerInfo.h"

HitEffect::HitEffect()
{
	_hitEffectTimer.isRepeat = false;
	_hitEffectTimer.duration = 0.5f;
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
