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

	const uint64 frame = 16;
	static auto updateTick = 0;

	auto currentTick = ::GetTickCount64();

	if (updateTick > currentTick)
		return;

	updateTick = currentTick + frame;

	UpdateRound();
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
		}
		else
		{
			_playerObjs[index]->AddComponent<PlayerInfo>(info);
			_players.insert({ info->GetPlayerUID(), _playerObjs[index] });
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

	SetTeamColor(mesh, _myObj->GetComponent<PlayerInfo>()->GetPlayerTeam());

	for (auto& [uid, player] : _players)
	{
		player->GetComponent<PlayerInfo>()->Init();
		player->SetSelfActive(true);

		HDData::SkinnedMeshRenderer* mesh = nullptr;

		PlayerInfo* info = player->GetComponent<PlayerInfo>();
		mesh = player->GetComponentInChildren<HDData::SkinnedMeshRenderer>();

		SetTeamColor(mesh, info->GetPlayerTeam());
	}


}

void RoundManager::UpdateRound()
{
	
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

void RoundManager::RecvOtherPlayerShoot(eHITLOC location)
{
	_myObj->GetComponent<PlayerInfo>()->OtherPlayerShoot(location);
}

void RoundManager::SendJump(int uid)
{
	NetworkManager::Instance().SendPlayJump();
}

void RoundManager::SetTeamColor(HDData::SkinnedMeshRenderer* mesh, eTeam color)
{
	switch (color)
	{
		case eTeam::R:
		{
			auto mat = API::GetMaterial("TP_Red");
			mesh->LoadMaterial(mat, 0);
			mesh->LoadMaterial(mat, 1);
			mesh->LoadMaterial(mat, 2);
			mesh->LoadMaterial(mat, 3);
			mesh->LoadMaterial(mat, 4);
		}
		break;
		case eTeam::G:
		{
			auto mat = API::GetMaterial("TP_Green");
			mesh->LoadMaterial(mat, 0);
			mesh->LoadMaterial(mat, 1);
			mesh->LoadMaterial(mat, 2);
			mesh->LoadMaterial(mat, 3);
			mesh->LoadMaterial(mat, 4);
		}
		break;
		case eTeam::B:
		{
			auto mat = API::GetMaterial("TP_Blue");
			mesh->LoadMaterial(mat, 0);
			mesh->LoadMaterial(mat, 1);
			mesh->LoadMaterial(mat, 2);
			mesh->LoadMaterial(mat, 3);
			mesh->LoadMaterial(mat, 4);
		}
		break;
		default:
		{
		}
		break;
	}
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
}

void RoundManager::SetRoundTimer(int time)
{
	_timer = time;
}

int& RoundManager::GetRoundTimer()
{
	return _timer;
}

void RoundManager::SetDesiredKill(int count)
{
	_desiredKill = count;
}

int& RoundManager::GetDesiredKill()
{
	return _desiredKill;
}

void RoundManager::SetMyKillCountUI(HDData::TextUI* txt)
{
	_myKillCount = txt;
}

void RoundManager::SetOthersKillCount(HDData::TextUI* txt, int index)
{
	_othersKillCount[index] = txt;
}

void RoundManager::SetAnimationDummy(HDData::GameObject* obj)
{
	_animationDummy = obj;
}

HDData::GameObject* RoundManager::GetAnimationDummy()
{
	return _animationDummy;
}

