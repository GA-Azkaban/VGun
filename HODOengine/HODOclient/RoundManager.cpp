#include "RoundManager.h"
#include "NetworkManager.h"
#include "LobbyManager.h"
#include "PlayerMove.h"
#include "GameManager.h"
#include "FPAniScript.h"
#include "MeshTransformController.h"
#include "CameraMove.h"
#include "GameManager.h"

RoundManager* RoundManager::_instance = nullptr;

RoundManager* RoundManager::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new RoundManager;
	}

	return _instance;
}

RoundManager::RoundManager()
{
	API::CreateStaticComponent(this);
}

void RoundManager::Start()
{

}

void RoundManager::Update()
{
	if (!_isRoundStart) return;

	UpdateRound();

	const uint64 frame = 16;
	static auto updateTick = 0;

	auto currentTick = ::GetTickCount64();

	if (updateTick > currentTick)
		return;

	updateTick = currentTick + frame;

	NetworkManager::Instance().SendPlayUpdate();
}

void RoundManager::SetRoundScene(HDData::Scene* scene)
{
	_roundScene = scene;
}

void RoundManager::InitGame()
{
	// 라운드 초기화
	auto& obj = LobbyManager::Instance().GetPlayerObjects();
	_playerNum = LobbyManager::Instance().GetPlayerNum();

	for (auto& obj : _playerObjs)
	{
		obj->SetSelfActive(false);
	}

	_players.clear();

	int index = 0;

	for (auto& one : obj)
	{
		if (index >= _playerNum) break;

		PlayerInfo* info = one->GetComponent<PlayerInfo>();

		if (info->GetPlayerUID() == GameManager::Instance()->GetMyInfo()->GetPlayerUID())
		{
			_myObj->AddComponent<PlayerInfo>(info)->SetIsMyInfo(true);
			GameManager::Instance()->SetMyObject(_myObj);
			_killCountObjs[index].first->SetText(GameManager::Instance()->GetMyInfo()->GetPlayerNickName());
			_inGameKillCounts.insert({ info->GetPlayerUID(), _killCountObjs[index] });
		}
		else
		{
			_playerObjs[index]->AddComponent<PlayerInfo>(info);
			_players.insert({ info->GetPlayerUID(), _playerObjs[index] });
			_killCountObjs[index].first->SetText(info->GetPlayerNickName());
			_inGameKillCounts.insert({ info->GetPlayerUID(), _killCountObjs[index] });
		}

		++index;
	}

	InitRound();
}

void RoundManager::EndGame()
{

}

void RoundManager::InitRound()
{
	// 타이머 초기화
	this->_timer = 0;

	HDData::SkinnedMeshRenderer* mesh = nullptr;
	mesh = _myObj->GetGameObjectByNameInChildren("meshShell")->GetComponentInChildren<HDData::SkinnedMeshRenderer>();

	_myObj->GetComponent<PlayerInfo>()->Init();
	_myObj->SetSelfActive(true);

	for (auto& [uid, player] : _players)
	{
		player->GetComponent<PlayerInfo>()->Init();
		player->SetSelfActive(true);

		HDData::SkinnedMeshRenderer* mesh = nullptr;

		PlayerInfo* info = player->GetComponent<PlayerInfo>();
		mesh = player->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
	}


}

void RoundManager::UpdateRound()
{
	UpdateRoundTimer();
	UpdateHPText();
	UpdateDesiredKillChecker();
}

void RoundManager::CheckHeadColliderOwner(HDData::DynamicSphereCollider* collider)
{
	int uid = collider->GetParentCollider()->GetGameObject()->GetComponent<PlayerInfo>()->GetPlayerUID();

	NetworkManager::Instance().SendPlayShoot(collider->GetTransform(), uid, Protocol::HIT_LOCATION_HEAD);
}

void RoundManager::CheckBodyColliderOwner(HDData::DynamicCapsuleCollider* collider)
{
	int uid = collider->GetGameObject()->GetComponent<PlayerInfo>()->GetPlayerUID();

	NetworkManager::Instance().SendPlayShoot(collider->GetTransform(), uid, Protocol::HIT_LOCATION_BODY);
}

void RoundManager::SendJump(int uid)
{
	NetworkManager::Instance().SendPlayJump();
}

std::unordered_map<int, HDData::GameObject*>& RoundManager::GetPlayerObjs()
{
	return _players;
}

int RoundManager::GetPlayerNum()
{
	return _playerNum;
}

bool RoundManager::GetIsRoundStart()
{
	return _isRoundStart;
}

void RoundManager::SetIsRoundStart(bool isStart)
{
	_isRoundStart = isStart;
}

void RoundManager::SetRoundTimerObject(HDData::TextUI* obj)
{
	_timerUI = obj;
	_timerUI->SetFont("Resources/Font/KRAFTON_55.spriteFont");
	_timerUI->GetTransform()->SetPosition(1350.0f, 60.0f, 0.0f);
}

void RoundManager::SetRoundTimer(int time)
{
	_timer = time;
}

void RoundManager::SetStartTime(std::chrono::time_point<std::chrono::steady_clock> time)
{
	_start_time = time;
}

int& RoundManager::GetRoundTimer()
{
	return _timer;
}

void RoundManager::UpdateRoundTimer()
{
	if (_isRoundStart)
	{
		auto currentTime = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsedTime = currentTime - _start_time;
		_timerUI->SetText(ChangeSecToMin(static_cast<int>(_timer - elapsedTime.count())));
		if (elapsedTime.count() >= _timer)
		{
			_isRoundStart = false;
		}
	}
}

std::string RoundManager::ChangeSecToMin(int second)
{
	int min = second / 60;
	int sec = second % 60;

	std::string result = std::to_string(min) + " : " + std::to_string(sec);

	return result;
}

void RoundManager::SetHPObject(HDData::TextUI* txt)
{
	_hpUI = txt;
}

void RoundManager::UpdateHPText()
{
	_hpUI->SetText(std::to_string(GameManager::Instance()->GetMyInfo()->GetPlayerCurrentHP()));
}

void RoundManager::UpdateDesiredKillChecker()
{
	{
		int count = _myObj->GetComponent<PlayerInfo>()->GetPlayerKillCount();
		_inGameKillCounts[GameManager::Instance()->GetMyInfo()->GetPlayerUID()].second->SetText(std::to_string(count));
		if (count >= _desiredKill) _winnerUID = GameManager::Instance()->GetMyInfo()->GetPlayerUID();
	}

	for (auto& [uid, player] : _players)
	{
		int count = player->GetComponent<PlayerInfo>()->GetPlayerKillCount();
		_inGameKillCounts[uid].second->SetText(std::to_string(count));
		if (count >= _desiredKill) _winnerUID = uid;
	}
}

void RoundManager::SetDesiredKill(int count)
{
	_desiredKill = count;
}

int& RoundManager::GetDesiredKill()
{
	return _desiredKill;
}

void RoundManager::SetKillCountUI(HDData::TextUI* nick, HDData::TextUI* count, int index)
{
	_killCountObjs[index] = std::make_pair(nick, count);
}

std::unordered_map<int, std::pair<HDData::TextUI*, HDData::TextUI*>>& RoundManager::GetKillCountMap()
{
	return _inGameKillCounts;
}

void RoundManager::SetAnimationDummy(HDData::GameObject* obj)
{
	_animationDummy = obj;
}

HDData::GameObject* RoundManager::GetAnimationDummy()
{
	return _animationDummy;
}

