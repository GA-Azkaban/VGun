#include "RoundManager.h"
#include "NetworkManager.h"
#include "LobbyManager.h"
#include "PlayerMove.h"
#include "GameManager.h"
#include "FPAniScript.h"
#include "MeshTransformController.h"
#include "CameraMove.h"

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
	NetworkManager::Instance().SendPlayUpdate();
}

void RoundManager::SetRoundScene(HDData::Scene* scene)
{
	_roundScene = scene;
}

void RoundManager::InitGame()
{
	// 라운드 초기화
	auto& data = LobbyManager::Instance().GetPlayerObjects();
	_playerNum = LobbyManager::Instance().GetPlayerNum();

	for (int i = 0; i < _playerNum; ++i)
	{
		PlayerInfo* info = data[i]->GetComponent<PlayerInfo>();

		_playerObjs[i]->AddComponent<PlayerInfo>(info);

		if (info->GetPlayerNickName() == GameManager::Instance()->GetMyInfo()->GetPlayerNickName())
		{
			GameManager::Instance()->SetMyInfo(_playerObjs[i]->GetComponent<PlayerInfo>());
		}
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

	// 플레이어 정보 초기화
	for (int i = 0; i < _playerNum; ++i)
	{
		_playerObjs[i]->GetComponent<PlayerInfo>()->Init();
	}

	for (int i = 0; i < _playerNum; ++i)
	{
		_playerObjs[i]->SetSelfActive(true);
		auto mesh = _playerObjs[i]->GetComponentInChildren<HDData::SkinnedMeshRenderer>();

		switch (_playerObjs[i]->GetComponent<PlayerInfo>()->GetPlayerTeam())
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
				break;
		}
	}

	_isRoundStart = true;
}

void RoundManager::UpdateRound()
{



	// 플레이어 transform 정보 갱신

	// 플레이어 state 정보 갱신

	// 플레이어 killcount, time 갱신

	// 플레이어 상태 (체력, 남은 총알 수, 위치) 를 서버에서 받아와 갱신
}

std::vector<HDData::GameObject*>& RoundManager::GetPlayerObjs()
{
	return _playerObjs;
}

void RoundManager::SetIsRoundStart(bool isStart)
{
	_isRoundStart = isStart;
}

