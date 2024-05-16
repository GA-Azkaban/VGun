﻿#include <algorithm>
#include "pch.h"
#include "Types.h"
#include "LobbyManager.h"
#include "NetworkManager.h"
#include "../HODOengine/TweenTimer.h"


LobbyManager& LobbyManager::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new LobbyManager;
	}

	return *_instance;
}

LobbyManager* LobbyManager::_instance = nullptr;

LobbyManager::LobbyManager()
{
	API::CreateStaticComponent(this);

	_roomData = new RoomData;
}

void LobbyManager::Start()
{

}

void LobbyManager::Update()
{

}

void LobbyManager::Login(std::string id, std::string password)
{
	std::string testid = id;
	std::string testpw = password;
	NetworkManager::Instance().SendLogin(id, password);
}

void LobbyManager::Join()
{
	_joinCanvas->OnEnable();
}

void LobbyManager::ExitJoin()
{
	_joinCanvas->OnDisable();
}

void LobbyManager::MakeNewAccount(std::string id, std::string password, std::string nickname)
{
	NetworkManager::Instance().SendCreateAccount(id, password, nickname);
}

void LobbyManager::LoginFAIL(int errorCode)
{
	switch (errorCode)
	{
		case LOGIN_FAIL:
			_loginFailCanvas->OnEnable();
			break;

		case ID_DUPLICATION:
			_iddupl->OnEnable();
			break;

		case NICKNAME_DUPLICATION:
			_namedupl->OnEnable();
			break;

		case SIGNUP_FAIL:
			_loginFailCanvas->OnEnable();
			break;

		default:
			std::cout << "Unknown Error Code" << std::endl;
			break;
	}
}

void LobbyManager::LoginSucess(int uid, std::string nickname)
{
	_loginSucessCanvas->OnEnable();
}

void LobbyManager::SetidDupl(HDData::GameObject* iddupl)
{
	_iddupl = iddupl;
}

void LobbyManager::SetnameDule(HDData::GameObject* namedupl)
{
	_namedupl = namedupl;
}

void LobbyManager::SetSignupFail(HDData::GameObject* signupfail)
{
	_signupFail = signupfail;
}

void LobbyManager::showOff(HDData::GameObject* object)
{
	object->OnDisable();
}

void LobbyManager::showOn(HDData::GameObject* object)
{
	object->OnEnable();
}

void LobbyManager::SetLobbyMainCanvas(HDData::GameObject* mainCanvas)
{
	_mainCanvas = mainCanvas;
}

void LobbyManager::SetFadeCanvas(HDData::GameObject* fadeCanvas)
{
	_fadeCanvas = fadeCanvas;
}

void LobbyManager::SetJoinCanvas(HDData::GameObject* joinCanvas)
{
	_joinCanvas = joinCanvas;
}

void LobbyManager::SetSucessCanvas(HDData::GameObject* sucessCanvas)
{
	_loginSucessCanvas = sucessCanvas;
}

void LobbyManager::SetFailCanvas(HDData::GameObject* failCanvas)
{
	_loginFailCanvas = failCanvas;
}

RoomData* LobbyManager::GetRoomData()
{
	return _roomData;
}

void LobbyManager::RoomEnter()
{
	NetworkManager::Instance().SendRoomEnter("room");
}

void LobbyManager::RoomEnterFAIL(int errorCode)
{
	switch (errorCode)
	{
		case 1:
		{
			_roomEnterFailCanvas->SetSelfActive(true);
		}
		break;
		case 2:
		{
			_roomPasswordFailCanvas->SetSelfActive(true);
		}
		break;
		default:
			break;
	}
}

void LobbyManager::RoomEnterSUCCESS()
{
	HDData::Scene* room = API::LoadSceneByName("Lobby");

	_players = _roomData->_players;

	for (int i = 0; i < _players.size(); ++i)
	{
		_playerObjs[i]->SetSelfActive(true);
	}
}

// TODO) 새로운 플레이어가 들어오거나 나갈 때, 기존 애니메이션이 끊기지 않으면 좋다.
// 하지만 지금은 작업 효율을 위해 일단 벡터를 갈아끼운다.
void LobbyManager::OtherPlayerEnter()
{
	_players = _roomData->_players;

	int count = _roomData->currentPlayerCount;

	for (int i = 0; i < 6; ++i)
	{
		//if (i < count)
		//{
		//	_playerObjs[i]->SetSelfActive(true);
		//	_nickNameIndex[i]->SetSelfActive(true);
		//	_nickNameIndex[i]->GetComponent<HDData::TextUI>()->SetText(_players[i]->GetPlayerNickName());
		//}
		//else
		//{
		//	_playerObjs[i]->SetSelfActive(false);
		//	_nickNameIndex[i]->SetSelfActive(false);
		//}
	}
}

void LobbyManager::OtherPlayerExit()
{
	_players = _roomData->_players;

	int count = _roomData->currentPlayerCount;

	for (int i = 0; i < 6; ++i)
	{
		_playerObjs[i]->SetSelfActive(false);
		_nickNameIndex[i]->SetSelfActive(false);
	}

	for (int i = 0; i < count; ++i)
	{
		_playerObjs[i]->SetSelfActive(true);
		SetPlayerTeam(_players[i]->GetPlayerTeam(), _playerObjs[i]);
		_nickNameIndex[i]->SetSelfActive(true);
		_nickNameIndex[i]->GetComponent<HDData::TextUI>()->SetText(_players[i]->GetPlayerNickName());
	}

}

void LobbyManager::SetPlayerTeam(eTeam team, HDData::GameObject* obj)
{
	auto mesh = obj->GetComponentInChildren<HDData::SkinnedMeshRenderer>();

	switch (team)
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

std::vector<HDData::GameObject*>& LobbyManager::GetPlayerObjects()
{
	return _playerObjs;
}

std::vector<HDData::GameObject*>& LobbyManager::GetNickNameObjects()
{
	return _nickNameIndex;
}

std::vector<HDData::GameObject*>& LobbyManager::GetTeamButtonObjects()
{
	return _teamButton;
}

