﻿#include <algorithm>
#include "pch.h"
#include "Types.h"
#include "LobbyManager.h"
#include "NetworkManager.h"
#include "GameManager.h"


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

void LobbyManager::SetSignupSuccess(HDData::GameObject* obj)
{
	_signupSuccess = obj;
}

void LobbyManager::SetInGameStartButton(HDData::GameObject* button)
{
	_inGameStartButton = button;
}

void LobbyManager::ShowSignSuccess()
{
	_signupSuccess->SetSelfActive(true);
}

RoomData* LobbyManager::GetRoomData()
{
	return _roomData;
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
}


void LobbyManager::RoomLeaveSuccess()
{
	GameManager::Instance()->GetMyInfo()->SetIsHost(false);
	API::LoadSceneByName("MainMenu");
}

void LobbyManager::RefreshRoom()
{
	for (int i = 0; i < 6; ++i)
	{
		_playerObjs[i]->SetSelfActive(false);
		_nickNameIndex[i]->SetSelfActive(false);
	}

	std::vector<PlayerInfo*>& data = _roomData->_players;
	_playerNum = data.size();

	for (int i = 0; i < _playerNum; ++i)
	{
		// 오브젝트 활성화, 캐릭터 타입
		_playerObjs[i]->SetSelfActive(true);
		auto mesh = _playerObjs[i]->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
		int type = data[i]->GetPlayerUID() % 8;
		mesh->LoadMesh(GameManager::Instance()->meshes[type]);
		mesh->LoadMaterial(API::GetMaterial("PolygonWestern_Texture_01_A"));
		data[i]->meshtype = type;

		// 닉네임 색깔 설정
		_nickNameIndex[i]->SetSelfActive(true);
		auto text = _nickNameIndex[i]->GetComponent<HDData::TextUI>();
		text->SetText(data[i]->GetPlayerNickName());
		text->SetColor(DirectX::Colors::White);

		if (GameManager::Instance()->GetMyInfo()->GetPlayerUID() == data[i]->GetPlayerUID())
		{
			text->SetColor(DirectX::Colors::Gold);

			if (data[i]->GetIsHost())	
				_inGameStartButton->SetSelfActive(true);
			else
				_inGameStartButton->SetSelfActive(false);
		}
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

int LobbyManager::GetPlayerNum()
{
	return _playerNum;
}

