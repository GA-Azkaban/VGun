#include <algorithm>
#include "pch.h"
#include "Types.h"
#include "LobbyManager.h"
#include "NetworkManager.h"
#include "GameManager.h"
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

//void LobbyManager::SetInGameReadyButton(HDData::GameObject* button)
//{
//	_inGameReadyButton = button;
//}

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

	auto& data = _roomData->_players;
	_playerNum = data.size();

	for (int i = 0; i < _roomData->_players.size(); ++i)
	{
		PlayerInfo* info = _playerObjs[i]->GetComponent<PlayerInfo>();
		info->SetNickName(data[i]->GetPlayerNickName());
		info->SetIsHost(data[i]->GetIsHost());
		info->SetTeamID(data[i]->GetPlayerTeam());
	}

	for (int i = 0; i < _roomData->_players.size(); ++i)
	{
		PlayerInfo* info = _playerObjs[i]->GetComponent<PlayerInfo>();

		_playerObjs[i]->SetSelfActive(true);
		_nickNameIndex[i]->GetComponent<HDData::TextUI>()->SetText(info->GetPlayerNickName());
		

		if (GameManager::Instance()->GetMyInfo()->GetIsHost())
		{
			_teamButton[i]->SetSelfActive(true);
			_quitButtons[i]->SetSelfActive(true);
			_inGameStartButton->SetSelfActive(true);
		}
		else if(info->GetPlayerNickName() == GameManager::Instance()->GetMyInfo()->GetPlayerNickName())
		{
			_teamButton[i]->SetSelfActive(true);
			_quitButtons[i]->SetSelfActive(false);
		}
	}
	
}


void LobbyManager::RefreshRoom()
{
	for (int i = 0; i < 6; ++i)
	{
		_playerObjs[i]->SetSelfActive(false);
		_nickNameIndex[i]->SetSelfActive(false);
		_teamButton[i]->SetSelfActive(false);
		_quitButtons[i]->SetSelfActive(false);
	}

	auto& data = _roomData->_players;
	_playerNum = data.size();

	for (int i = 0; i < _roomData->_players.size(); ++i)
	{
		PlayerInfo* info = _playerObjs[i]->GetComponent<PlayerInfo>();
		info->SetNickName(data[i]->GetPlayerNickName());
		info->SetIsHost(data[i]->GetIsHost());
		info->SetTeamID(data[i]->GetPlayerTeam());
	}

	for (int i = 0; i < _roomData->_players.size(); ++i)
	{
		PlayerInfo* info = _playerObjs[i]->GetComponent<PlayerInfo>();

		_playerObjs[i]->SetSelfActive(true);
		
		switch (info->GetPlayerTeam())
		{
			case eTeam::R :
			{
				SetPlayerTeam(eTeam::R, info->GetPlayerNickName());
			}
			break;
			case eTeam::G:
			{
				SetPlayerTeam(eTeam::G, info->GetPlayerNickName());
			}
			break;
			case eTeam::B:
			{
				SetPlayerTeam(eTeam::B, info->GetPlayerNickName());
			}
			break;
			default:
				break;
		}

		_nickNameIndex[i]->SetSelfActive(true);
		_nickNameIndex[i]->GetComponent<HDData::TextUI>()->SetText(info->GetPlayerNickName());

		if (GameManager::Instance()->GetMyInfo()->GetIsHost())
		{
			_teamButton[i]->SetSelfActive(true);
			_quitButtons[i]->SetSelfActive(true);
			_inGameStartButton->SetSelfActive(true);
		}
		else if (info->GetPlayerNickName() == GameManager::Instance()->GetMyInfo()->GetPlayerNickName())
		{
			_teamButton[i]->SetSelfActive(true);
			_quitButtons[i]->SetSelfActive(false);
		}
	}
}

void LobbyManager::SetPlayerTeam(eTeam team, std::string nickName)
{
	HDData::SkinnedMeshRenderer* mesh = nullptr;

	for (int i = 0; i < _roomData->_players.size(); ++i)
	{
		PlayerInfo* info = _playerObjs[i]->GetComponent<PlayerInfo>();

		if (nickName == info->GetPlayerNickName())
		{
			mesh = _playerObjs[i]->GetComponentInChildren<HDData::SkinnedMeshRenderer>();
		}
	}

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

std::vector<HDData::GameObject*>& LobbyManager::GetQuitButtonObjects()
{
	return _quitButtons;
}

int LobbyManager::GetPlayerNum()
{
	return _playerNum;
}

