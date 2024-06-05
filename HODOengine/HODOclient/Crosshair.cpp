#include "Crosshair.h"
#include "PlayerMove.h"

Crosshair::Crosshair()
{
	_hitCrosshairTimer.isRepeat = false;
	_hitCrosshairTimer.duration = 0.5f;
	_hitCrosshairTimer.onExpiration = [&]() { _hitCrosshair->SetActive(false); };

	_criticalCrosshairTimer.isRepeat = false;
	_criticalCrosshairTimer.duration = 0.5f;
	_criticalCrosshairTimer.onExpiration = [&]() {_criticalCrosshair->SetActive(false); };
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
}

void Crosshair::Update()
{
	if (playerMove != nullptr)
	{
		if (playerMove->IsShootHead())
		{
			_criticalCrosshair->SetActive(true);
			_criticalCrosshairTimer.Start();
		}

		if (playerMove->IsShootBody())
		{
			_hitCrosshair->SetActive(true);
			_hitCrosshairTimer.Start();
		}
	}

	_criticalCrosshairTimer.Update();
	_hitCrosshairTimer.Update();
}
