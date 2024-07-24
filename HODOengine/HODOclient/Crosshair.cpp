#include "Crosshair.h"
#include "PlayerMove.h"
#include "GameManager.h"
#include "PlayerInfo.h"

Crosshair::Crosshair()
{
	_hitTimer.isRepeat = false;
	_hitTimer.duration = 0.5f;
	_hitTimer.onExpiration = [&]() { _hitCrosshair->SetActive(false); };

	_criticalTimer.isRepeat = false;
	_criticalTimer.duration = 0.5f;
	_criticalTimer.onExpiration = [&]() {_criticalCrosshair->SetActive(false); };
}

void Crosshair::Start()
{
	_defaultCrosshair = GetGameObject()->AddComponent<HDData::ImageUI>();
	_defaultCrosshair->GetTransform()->SetPosition(API::GetScreenWidth() / 2.0f, API::GetScreenHeight() / 2.0f, 0);
	_defaultCrosshair->SetImage("Crosshair15White.png");
	_defaultCrosshair->ChangeScale(0.25f, 0.25f);

	_criticalCrosshair = GetGameObject()->AddComponent<HDData::ImageUI>();
	_criticalCrosshair->GetTransform()->SetPosition(API::GetScreenWidth() / 2.0f, API::GetScreenHeight() / 2.0f, 0);
	_criticalCrosshair->SetImage("Crosshair14Red.png");
	_criticalCrosshair->ChangeScale(0.30f, 0.30f);
	_criticalCrosshair->SetActive(false);

	_hitCrosshair = GetGameObject()->AddComponent<HDData::ImageUI>();
	_hitCrosshair->GetTransform()->SetPosition(API::GetScreenWidth() / 2.0f, API::GetScreenHeight() / 2.0f, 0);
	_hitCrosshair->SetImage("Crosshair14White.png");
	_hitCrosshair->ChangeScale(0.275f, 0.275f);
	_hitCrosshair->SetActive(false);

	GameManager::Instance()->GetMyInfo()->SetCrosshairUI(this);
}

void Crosshair::Update()
{
	if (playerMove != nullptr)
	{
		if (playerMove->IsShootHead())
		{
			_criticalCrosshair->SetActive(true);
			_criticalTimer.Start();
		}

		if (playerMove->IsShootBody())
		{
			_hitCrosshair->SetActive(true);
			_hitTimer.Start();
		}
	}

	_criticalTimer.Update();
	_hitTimer.Update();
}

void Crosshair::SetActive(bool isActive)
{
	_defaultCrosshair->SetActive(isActive);

	// 켜줄 때 Default crosshair만 켜주기
	if (isActive)
	{
		return;
	}
	_criticalCrosshair->SetActive(isActive);
	_hitCrosshair->SetActive(isActive);
}
