#include "Ammo.h"
#include "PlayerMove.h"

Ammo::Ammo()
	:_defaultAmmo(),
	_remainningAmmo(),
	_bulletCount(),
	playerMove()
{

}

void Ammo::Start()
{
	//_defaultAmmo = GetGameObject()->AddComponent<HDData::TextUI>();
	//_defaultAmmo->GetTransform()->SetPosition(2400.0f,1400.0f,0.0f);
	//_defaultAmmo->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	//_defaultAmmo->SetText("/ 30");

	_remainningAmmo = GetGameObject()->AddComponent<HDData::TextUI>();
	_remainningAmmo->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	_remainningAmmo->GetTransform()->SetPosition(2250.0f, 1400.0f, 0.0f);
} 

void Ammo::Update()
{
	_bulletCount = playerMove->GetBulletCount();
	std::string temp = std::to_string(_bulletCount);
	_remainningAmmo->SetText(temp);
}

