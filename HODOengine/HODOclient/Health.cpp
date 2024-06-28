#include "Health.h"
#include "GameManager.h"

Health::Health()
	:_playerHealth(),
	_healthPoint(GameManager::Instance()->GetMyInfo()->GetPlayerCurrentHP())
{

}

void Health::Start()
{
	_playerHealth = GetGameObject()->AddComponent<HDData::TextUI>();
	_playerHealth->GetTransform()->SetPosition(2000.0f,1400.0f,0.0f);
	_playerHealth->SetFont("Resources/Font/KRAFTON_55.spriteFont");
}

void Health::Update()
{
	_playerHealth->SetText(std::to_string(_healthPoint));
}
