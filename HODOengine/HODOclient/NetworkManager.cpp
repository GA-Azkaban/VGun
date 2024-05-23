﻿#include "pch.h"
#include <string>
#include "NetworkManager.h"

#include "Service.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"

#include "RoundManager.h"
#include "LobbyManager.h"
#include "GameManager.h"
#include "MenuManager.h"
#include "GameStruct.h"
#include "ErrorCode.h"

NetworkManager& NetworkManager::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new NetworkManager;
	}

	return *_instance;
}

NetworkManager* NetworkManager::_instance = nullptr;

NetworkManager::NetworkManager()
{
	API::CreateStaticComponent(this);
}

void NetworkManager::Start()
{
	ServerPacketHandler::Init();

	_service = Horang::MakeShared<Horang::ClientService>(
		Horang::NetAddress(L"172.16.1.13", 7777),
		Horang::MakeShared<Horang::IocpCore>(),
		Horang::MakeShared<ServerSession>,
		1
	);

	_service->Start();
}

void NetworkManager::Update()
{
	_service->GetIocpCore()->Dispatch(0);
}

void NetworkManager::Connected()
{
	_isConnect = true;

#if _DEBUG
	FILE* pFile = nullptr;

	if (AllocConsole())
		freopen_s(&pFile, "CONOUT$", "w", stdout);
	else
		ASSERT_CRASH(false);

	std::cout << "Connected" << std::endl;
#endif


}

void NetworkManager::Disconnected()
{
	_isConnect = false;
	std::cout << "Disconnect" << std::endl;
}

void NetworkManager::SendLogin(std::string id, std::string password)
{
	Protocol::C_SIGNIN packet;
	packet.set_id(id);
	packet.set_password(password);

	PlayerInfo* player = new PlayerInfo;
	player->SetPlayerID(id);

	GameManager::Instance()->SetMyInfo(player);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::SendCreateAccount(std::string id, std::string password, std::string nickname)
{
	Protocol::C_SIGNUP packet;
	packet.set_id(id);
	packet.set_password(password);
	packet.set_nickname(nickname);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvFail(int32 errorCode)
{
	// Todo
	// 로그인에 실패했을때 모든 처리를 여기서
	LobbyManager::Instance().LoginFAIL(errorCode);

	// 방 입장에 실패했을 때
	LobbyManager::Instance().RoomEnterFAIL(errorCode);
}

void NetworkManager::SendAutoLogin()
{
	Protocol::C_AUTOLOGIN packet;

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvLogin(int32 uid, std::string nickName)
{
	// remove pyramid
	// 로그인이 성공했을때 처리
	LobbyManager::Instance().LoginSucess(uid, nickName);

	GameManager::Instance()->GetMyInfo()->SetNickName(nickName);

	API::LoadSceneByName("MainMenu");
}

void NetworkManager::RecvCreateAccount()
{
	LobbyManager::Instance().ShowSignSuccess();
}

void NetworkManager::SendRoomListRequest()
{
	Protocol::C_ROOM_LIST_REQUEST packet;

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvRoomList(Protocol::S_ROOM_LIST roomList)
{
	auto& list = MenuManager::Instance().GetRoomList();

	list.clear();

	for (auto& room : roomList.roominfo())
	{
		roominfo* info = new roominfo;

		info->id = room.roomid();
		info->title = room.roomname();
		info->maxPlayerCount = room.maxplayercount();
		info->currentPlayerCount = room.currentplayercount();

		info->isPrivate = room.isprivate();
		info->isTeam = room.isteam();

		list.push_back(info);
	}

	MenuManager::Instance().RenderRoomList();
}

void NetworkManager::SendRoomEnter(std::string roomCode, std::string password /*= ""*/)
{
	Protocol::C_ROOM_ENTER packet;
	packet.set_roomcode(roomCode);
	packet.set_password(password);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::SendRoomLeave()
{
	Protocol::C_ROOM_LEAVE packet;

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvRoomEnter(Protocol::RoomInfo roomInfo)
{
	// Todo RoomInfo 설정
	auto info = LobbyManager::Instance().GetRoomData();

	info->_players.clear();

	info->roomid = roomInfo.roomid();

	info->isPrivate = roomInfo.isprivate();

	info->roomName = roomInfo.roomname();
	info->password = roomInfo.password();

	info->currentPlayerCount = roomInfo.currentplayercount();

	if (roomInfo.users().empty())
	{
		return;
	}

	for (auto& player : roomInfo.users())
	{
		PlayerInfo* one = new PlayerInfo;

		one->SetNickName(player.userinfo().nickname());
		one->SetIsHost(player.host());
		
		switch (player.team())
		{
			case Protocol::TEAM_COLOR_RED:
			{
				one->SetTeamID(eTeam::R);
			}
			break;
			case Protocol::TEAM_COLOR_GREEN:
			{
				one->SetTeamID(eTeam::G);
			}
			break;
			case Protocol::TEAM_COLOR_BLUE:
			{
				one->SetTeamID(eTeam::B);
			}
			break;
			default:
			{
				one->SetTeamID(eTeam::R);
				SendChangeTeamColor(Protocol::TEAM_COLOR_RED, player.userinfo().nickname());
			}
			break;
		}

		// 플레이어 정보 받기	
		info->_players.push_back(one);
	}

	LobbyManager::Instance().RoomEnterSUCCESS();
}

void NetworkManager::RecvRoomLeave(Protocol::RoomInfo roomInfo)
{
	API::LoadSceneByName("MainMenu");
}

void NetworkManager::SendRoomCreate(std::string roomName, std::string password /*= ""*/, int32 maxPlayerCount /*= 6*/, bool isPrivate /*= false*/, bool isTeam /*= true*/)
{
	Protocol::C_ROOM_CREATE packet;

	packet.set_roomname(roomName);
	packet.set_password(password);
	packet.set_maxplayercount(maxPlayerCount);
	packet.set_isprivate(isPrivate);
	packet.set_isteam(isTeam);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::SetRoom(Protocol::RoomInfo roomInfo)
{
	Protocol::C_ROOM_CREATE packet;

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvAnotherPlayerEnter(Protocol::RoomInfo roomInfo)
{
	auto info = LobbyManager::Instance().GetRoomData();

	info->_players.clear();

	for (auto& player : roomInfo.users())
	{
		PlayerInfo* one = new PlayerInfo;
		one->SetNickName(player.userinfo().nickname());
		one->SetIsHost(player.host());

		if (player.host() && (GameManager::Instance()->GetMyInfo()->GetPlayerNickName() == player.userinfo().nickname())) 
		{ GameManager::Instance()->GetMyInfo()->SetIsHost(true); }

		one->SetCurrentHP(player.hp());

		info->_players.push_back(one);
	}

	LobbyManager::Instance().RefreshRoom();
}

void NetworkManager::RecvAnotherPlayerLeave(Protocol::RoomInfo roomInfo)
{
	auto info = LobbyManager::Instance().GetRoomData();

	info->_players.clear();

	for (auto& player : roomInfo.users())
	{
		PlayerInfo* one = new PlayerInfo;
		one->SetNickName(player.userinfo().nickname());
		one->SetIsHost(player.host());
		one->SetCurrentHP(player.hp());

		info->_players.push_back(one);
	}

	LobbyManager::Instance().RefreshRoom();
}

void NetworkManager::SendKickPlayer(std::string targetNickName)
{
	Protocol::C_ROOM_KICK packet;

	packet.set_targetnickname(targetNickName);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvKickPlayer(Protocol::RoomInfo roomInfo)
{
	API::LoadSceneByName("MainMenu");
}

void NetworkManager::SendChangeTeamColor(Protocol::eTeamColor teamColor, std::string targetNickName)
{
	Protocol::C_ROOM_CHANGE_TEAM packet;

	packet.set_teamcolor(teamColor);
	packet.set_targetnickname(targetNickName);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvChangeTeamColor(Protocol::RoomInfo roomInfo)
{
	auto info = LobbyManager::Instance().GetRoomData();

	for (int i = 0; i < roomInfo.users().size(); ++i)
	{
		std::string test = roomInfo.users()[i].userinfo().nickname();

		for (int j = 0; j < roomInfo.users().size(); ++j)
		{

			std::string test2 = LobbyManager::Instance().GetAllPlayerInfo()[j]->GetPlayerNickName();

			if (roomInfo.users()[i].userinfo().nickname() == LobbyManager::Instance().GetAllPlayerInfo()[j]->GetPlayerNickName())
			{
				switch (roomInfo.users()[i].team())
				{
					case Protocol::TEAM_COLOR_RED:
					{
						std::string test = roomInfo.users()[j].userinfo().nickname();
						LobbyManager::Instance().SetPlayerTeam(eTeam::R, roomInfo.users()[j].userinfo().nickname());
					}
					break;
					case Protocol::TEAM_COLOR_GREEN:
					{
						std::string test = roomInfo.users()[j].userinfo().nickname();
						LobbyManager::Instance().SetPlayerTeam(eTeam::G, roomInfo.users()[j].userinfo().nickname());
					}
					break;
					case Protocol::TEAM_COLOR_BLUE:
					{
						std::string test = roomInfo.users()[j].userinfo().nickname();
						LobbyManager::Instance().SetPlayerTeam(eTeam::B, roomInfo.users()[j].userinfo().nickname());
					}
					break;
					default:
						break;
				}
			}
		}

		
	}
}

void NetworkManager::SendGameStart()
{
	Protocol::C_ROOM_START packet;

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvGameStart()
{
	RoundManager::Instance()->Start();
	auto currentScene = API::LoadSceneByName("InGame");
	auto mainLight = currentScene->GetMainLight();
	mainLight->SetDirection(Vector4{ -1, -3, -2, 0 });
}

void NetworkManager::SendPlayUpdate(Protocol::PlayerData playerData)
{
	Protocol::C_PLAY_UPDATE packet;

	// Todo PlayerData 설정

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvPlayUpdate(Protocol::S_PLAY_UPDATE playUpdate)
{

}

bool NetworkManager::IsConnected()
{
	return _isConnect;
}
