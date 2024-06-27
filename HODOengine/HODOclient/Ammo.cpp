#include "Ammo.h"
#include "GameManager.h"
#include "PlayerMove.h"

Ammo::Ammo()
	:_defaultAmmo(),
	_remainningAmmo(),
	_bulletCount(GameManager::Instance()->GetMyInfo()->GetCurrentBulletCount())
{

}

void Ammo::Start()
{
	_remainningAmmo = GetGameObject()->AddComponent<HDData::TextUI>();
	_remainningAmmo->SetFont("Resources/Font/Western_55.spriteFont");
	_remainningAmmo->GetTransform()->SetPosition(2250.0f, 1400.0f, 0.0f);
} 

void Ammo::Update()
{
	_remainningAmmo->SetText(std::to_string(_bulletCount));
}

