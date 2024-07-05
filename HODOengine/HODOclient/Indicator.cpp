#include "Indicator.h"
#include "IndicatorPool.h"

Indicator::Indicator()
{
	_indicatorTimer.isRepeat = false;
	_indicatorTimer.duration = 0.5f;
	_indicatorTimer.onExpiration = [&]() { GetGameObject()->SetSelfActive(false); };

	_indicatorImage = GetGameObject()->AddComponent<HDData::ImageUI>();
	_indicatorImage->GetTransform()->SetPosition(API::GetScreenWidth() / 2.0f, API::GetScreenHeight() / 2.0f, 0);
	_indicatorImage->SetImage("DamegeIndicator.png");
	//_indicatorImage->ChangeScale(1.5f, 1.5f);
	float width = _indicatorImage->GetImageWidth();
	float height = _indicatorImage->GetImageHeight();
	//_indicatorImage->SetOrigin(width / 2.0f * 0.65f, height / 2.0f * 0.7f);
	_indicatorImage->SetOrigin(width / 2.0f * 0.975f, height / 2.0f * 1.05f);
	_indicatorImage->SetCenter(0, 0);
	_indicatorImage->GetGameObject()->SetSelfActive(false);
}

Indicator* Indicator::CreateIndicator()
{
	auto obj = API::CreateObject(API::GetCurrenScene());
	auto comp = obj->AddComponent<Indicator>();
	return comp;
}

void Indicator::Update()
{
	/*static float angle = 0.0f;
	if (API::GetKeyPressing(DIK_U))
	{
		angle += API::GetDeltaTime();
	}
	if (API::GetKeyPressing(DIK_I))
	{
		angle -= API::GetDeltaTime();
	} */

	// 각도 계산
	//_indicatorImage->SetAngle(angle);
	_indicatorTimer.Update();
}

void Indicator::SetEffectOn(Vector3 targetPos)
{
	// 각도 계산
	_targetPos = targetPos;
	auto player = IndicatorPool::Instance().player;
	Vector3 forward = player->GetTransform()->GetForward();
	Vector3 targetDir = targetPos - player->GetTransform()->GetPosition();
	Vector3 targetDirNorm = targetDir.Normalize();


	_indicatorTimer.Start();
	//_indicatorImage->SetAngle();
}
