#include "DamageLog.h"
#include "GameManager.h"
#include "PlayerInfo.h"

DamageLog::DamageLog()
	: _logIndex(0)
{
	_curve.AddKey(0.0f, 0.625f, [](float t) { return 1.0f; });
	_curve.AddKey(0.625f, 1.0f, [](float t) { return -2.67f * t + 2.67f; });

	for (int i = 0; i < MAXLOGCOUNT; ++i)
	{
		_logTimer[i].duration = 8.0f;
		_logTimer[i].isRepeat = false;
		_logTimer[i].onUpdate = [&, i](float progress)
			{
				float alpha = _curve.Evaluate(progress);

				_nicknames[i]->SetColor({ 1.0f, 1.0f, 1.0f, alpha });
				_damages[i]->SetColor({ 1.0f, 0.0f, 0.0f, alpha });
				_remainHPs[i]->SetColor({ 0.0f, 56.0f / 255.0f, 90.0f / 255.0f, alpha });
			};
		_logTimer[i].onExpiration = [&, i]()
			{
				_nicknames[i]->SetActive(false);
				_damages[i]->SetActive(false);
				_remainHPs[i]->SetActive(false);
			};
	}

}

void DamageLog::DisplayLog(const std::string& nickname, int damage, int remainHP)
{
	for (int i = 0; i < MAXLOGCOUNT; ++i)
	{
		if (_nicknames[i]->GetIsActive())
		{
			break;
		}

		if (i == MAXLOGCOUNT - 1)
		{
			_logIndex = 0;
		}
	}

	_nicknames[_logIndex]->SetText(nickname);
	_nicknames[_logIndex]->SetActive(true);
	_nicknames[_logIndex]->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	_nicknames[_logIndex]->GetTransform()->SetPosition(_uiXPosition[0], _uiYPosition[_logIndex], 0);

	_damages[_logIndex]->SetText(std::to_string(damage));
	_damages[_logIndex]->SetActive(true);
	_damages[_logIndex]->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	_damages[_logIndex]->GetTransform()->SetPosition(_uiXPosition[1], _uiYPosition[_logIndex], 0);

	if (remainHP < 0)
	{
		remainHP = 0;
	}
	int percent = remainHP;
	_remainHPs[_logIndex]->SetText(std::to_string(percent) + "%");
	_remainHPs[_logIndex]->SetActive(true);
	_remainHPs[_logIndex]->SetColor({ 0.0f, 56.0f / 255.0f, 90.0f / 255.0f, 1.0f });
	_remainHPs[_logIndex]->GetTransform()->SetPosition(_uiXPosition[2], _uiYPosition[_logIndex], 0);

	_logTimer[_logIndex].Start();

	++_logIndex;
	_logIndex = _logIndex % MAXLOGCOUNT;
}

void DamageLog::Start()
{
	Vector3 position = GetGameObject()->GetTransform()->GetPosition();
	_uiXPosition = { position.x - 200, position.x, position.x + 100 };
	_uiYPosition = { position.y - 60, position.y, position.y + 60 };

	for (int i = 0; i < MAXLOGCOUNT; ++i)
	{
		auto nicknameObj = API::CreateObject(API::GetCurrentScene());
		_nicknames[i] = nicknameObj->AddComponent<HDData::TextUI>();
		_nicknames[i]->SetFont("Resources/Font/KRAFTON_30.spriteFont");
		_nicknames[i]->SetText(" ");
		_nicknames[i]->SetActive(false);

		auto damageObj = API::CreateObject(API::GetCurrentScene());
		_damages[i] = damageObj->AddComponent<HDData::TextUI>();
		_damages[i]->SetFont("Resources/Font/KRAFTON_40.spriteFont");
		_damages[i]->SetText(" ");
		_damages[i]->SetActive(false);

		auto remainHPObj = API::CreateObject(API::GetCurrentScene());
		_remainHPs[i] = remainHPObj->AddComponent<HDData::TextUI>();
		_remainHPs[i]->SetFont("Resources/Font/KRAFTON_40.spriteFont");
		_remainHPs[i]->SetText(" ");
		_remainHPs[i]->SetActive(false);
	}

	GameManager::Instance()->GetMyInfo()->SetDamageLogUI(this);
}

void DamageLog::Update()
{
	for (int i = 0; i < MAXLOGCOUNT; ++i)
	{
		_logTimer[i].Update();
	}
}

void DamageLog::SetActive(bool isActive)
{
	for (int i = 0; i < MAXLOGCOUNT; ++i)
	{
		_nicknames[i]->SetActive(isActive);
		_damages[i]->SetActive(isActive);
		_remainHPs[i]->SetActive(isActive);
	}
}
