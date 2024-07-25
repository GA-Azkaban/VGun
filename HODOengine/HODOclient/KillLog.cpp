#include "KillLog.h"
#include "GameManager.h"
#include "PlayerInfo.h"
#include "RoundManager.h"

KillLog::KillLog()
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
				_killerNicknames[i]->SetAlpha(alpha);
				_kills[i]->SetAlpha(alpha);
				_deadNicknames[i]->SetAlpha(alpha);
			};
		_logTimer[i].onExpiration = [&, i]()
			{
				_killerNicknames[i]->SetActive(false);
				_kills[i]->SetActive(false);
				_deadNicknames[i]->SetActive(false);
			};
	}

}

void KillLog::DisplayLog(const std::string& killerNickname, const std::string& deadNickname, KillLogType logType)
{
	for (int i = 0; i < MAXLOGCOUNT; ++i)
	{
		if (_killerNicknames[i]->GetIsActive())
		{
			break;
		}

		if (i == MAXLOGCOUNT - 1)
		{
			_logIndex = 0;
		}
	}

	_killerNicknames[_logIndex]->SetText(killerNickname);
	_killerNicknames[_logIndex]->SetActive(true);
	_killerNicknames[_logIndex]->GetTransform()->SetPosition(_uiXPosition[0], _uiYPosition[_logIndex], 0);

	_kills[_logIndex]->SetText("kill");
	_kills[_logIndex]->SetActive(true);
	_kills[_logIndex]->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	_kills[_logIndex]->GetTransform()->SetPosition(_uiXPosition[1], _uiYPosition[_logIndex], 0);

	_deadNicknames[_logIndex]->SetText(deadNickname);
	_deadNicknames[_logIndex]->SetActive(true);
	_deadNicknames[_logIndex]->GetTransform()->SetPosition(_uiXPosition[2], _uiYPosition[_logIndex], 0);

	switch (logType)
	{
		case KillLogType::PLAYERKILLENEMY:
		{
			_killerNicknames[_logIndex]->SetColor({ 0.0f, 56.0f / 255.0f, 90.0f / 255.0f, 1.0f });
			_deadNicknames[_logIndex]->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
		}
		break;
		case KillLogType::ENEMYKILLPLAYER:
		{
			_killerNicknames[_logIndex]->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
			_deadNicknames[_logIndex]->SetColor({ 0.0f, 56.0f / 255.0f, 90.0f / 255.0f, 1.0f });
		}
		break;
		case KillLogType::ENEMYKILLENEMY:
		{
			_killerNicknames[_logIndex]->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
			_deadNicknames[_logIndex]->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
		}
		break;
	}

	_logTimer[_logIndex].Start();

	++_logIndex;
	_logIndex = _logIndex % MAXLOGCOUNT;
}

void KillLog::Start()
{
	Vector3 position = GetGameObject()->GetTransform()->GetPosition();
	_uiXPosition = { position.x - 150, position.x, position.x + 150 };
	_uiYPosition = { position.y - 50, position.y, position.y + 50, position.y + 100 };

	for (int i = 0; i < MAXLOGCOUNT; ++i)
	{
		auto killerNicknameObj = API::CreateObject(API::GetCurrentScene());
		_killerNicknames[i] = killerNicknameObj->AddComponent<HDData::TextUI>();
		_killerNicknames[i]->SetFont("Resources/Font/KRAFTON_40.spriteFont");
		_killerNicknames[i]->SetText(" ");
		_killerNicknames[i]->SetActive(false);

		auto killObj = API::CreateObject(API::GetCurrentScene());
		_kills[i] = killObj->AddComponent<HDData::TextUI>();
		_kills[i]->SetFont("Resources/Font/KRAFTON_40.spriteFont");
		_kills[i]->SetText(" ");
		_kills[i]->SetActive(false);

		auto deadNicknameObj = API::CreateObject(API::GetCurrentScene());
		_deadNicknames[i] = deadNicknameObj->AddComponent<HDData::TextUI>();
		_deadNicknames[i]->SetFont("Resources/Font/KRAFTON_40.spriteFont");
		_deadNicknames[i]->SetText(" ");
		_deadNicknames[i]->SetActive(false);
	}

	GameManager::Instance()->GetMyInfo()->SetKillLogUI(this);
	RoundManager::Instance()->killLog = this;
}

void KillLog::Update()
{
	for (int i = 0; i < MAXLOGCOUNT; ++i)
	{
		_logTimer[i].Update();
	}
}

void KillLog::OnDisable()
{
	SetActive(false);
}

void KillLog::SetActive(bool isActive)
{
	for (int i = 0; i < MAXLOGCOUNT; ++i)
	{
		_killerNicknames[i]->SetActive(isActive);
		_kills[i]->SetActive(isActive);
		_deadNicknames[i]->SetActive(isActive);
	}
}
