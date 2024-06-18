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

	auto& playerObj = RoundManager::Instance()->GetPlayerObjs();

	if (playerObj.size() == 0) return;

	int index = 0;
	for (auto& [uid, player] : playerObj)
	{
		Interpolation(player->GetTransform(), serverPosition[index], serverRotation[index], 2.5);
		++index;
	}
}

void NetworkManager::RecvPlayShoot(Protocol::PlayerData playerData)
{
	int i = 3;
}

void NetworkManager::RecvPlayShoot(Protocol::PlayerData playerData, Protocol::PlayerData hitPlayerData, Protocol::eHitLocation hitLocation)
{
	auto uid = playerData.userinfo().uid();

	switch (hitLocation)
	{
		case Protocol::HIT_LOCATION_NONE:
			break;
		case Protocol::HIT_LOCATION_NO_HIT:
			break;
		case Protocol::HIT_LOCATION_HEAD:
		{
			RoundManager::Instance()->RecvOtherPlayerShoot(eHITLOC::HEAD);
		}
			break;
		case Protocol::HIT_LOCATION_BODY:
		{
			RoundManager::Instance()->RecvOtherPlayerShoot(eHITLOC::BODY);
		}
			break;
		case Protocol::HIT_LOCATION_ARM:
			break;
		case Protocol::HIT_LOCATION_LEG:
			break;
		case Protocol::eHitLocation_INT_MIN_SENTINEL_DO_NOT_USE_:
			break;
		case Protocol::eHitLocation_INT_MAX_SENTINEL_DO_NOT_USE_:
			break;
		default:
			break;
	}
}

void NetworkManager::RecvPlayKillDeath(Protocol::PlayerData deathPlayerData, Protocol::PlayerData killPlayerData)
{

}

void NetworkManager::RecvPlayRespawn(Protocol::PlayerData playerData)
{

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

	GameManager::Instance()->GetMyInfo()->SetPlayerUID(uid);
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
		one->SetPlayerUID(player.userinfo().uid());

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
	//if (roomInfo.currentplayercount() == 0)
	//{

	//}

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
		one->SetPlayerUID(player.userinfo().uid());
		one->SetCurrentHP(player.hp());

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

		if (player.host() && (GameManager::Instance()->GetMyInfo()->GetPlayerNickName() == player.userinfo().nickname()))
		{
			GameManager::Instance()->GetMyInfo()->SetIsHost(true);
		}

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
		one->SetPlayerUID(player.userinfo().uid());
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
	auto info = LobbyManager::Instance().GetRoomData();

	info->_players.clear();

	for (auto& player : roomInfo.users())
	{
		PlayerInfo* one = new PlayerInfo;
		one->SetPlayerID(player.userinfo().id());
		one->SetNickName(player.userinfo().nickname());
		one->SetIsHost(player.host());
		one->SetCurrentHP(player.hp());

		info->_players.push_back(one);
	}
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

			if (roomInfo.users()[i].userinfo().nickname() ==
				LobbyManager::Instance().GetPlayerObjects()[j]->GetComponent<PlayerInfo>()->GetPlayerNickName())
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
	RoundManager::Instance()->InitGame();
	API::LoadSceneByName("InGame");
	API::SetRecursiveMouseMode(true);
}

void NetworkManager::SendPlayUpdate()
{
	Protocol::C_PLAY_UPDATE packet;

	auto& playerobj = RoundManager::Instance()->_myObj;

	auto vector3 = packet.mutable_playerdata()->mutable_transform()->mutable_vector3();
	vector3->set_x(playerobj->GetTransform()->GetPosition().x);
	vector3->set_y(playerobj->GetTransform()->GetPosition().y);
	vector3->set_z(playerobj->GetTransform()->GetPosition().z);
	vector3;

	auto quaternion = packet.mutable_playerdata()->mutable_transform()->mutable_quaternion();
	quaternion->set_w(playerobj->GetTransform()->GetRotation().w);
	quaternion->set_x(playerobj->GetTransform()->GetRotation().x);
	quaternion->set_y(playerobj->GetTransform()->GetRotation().y);
	quaternion->set_z(playerobj->GetTransform()->GetRotation().z);
	quaternion;

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvPlayUpdate(Protocol::S_PLAY_UPDATE playUpdate)
{
	auto& playerobj = RoundManager::Instance()->GetPlayerObjs();
	auto& roominfo = playUpdate.roominfo();

	int index = 0;

	for (auto& player : roominfo.users())
	{
		if (player.userinfo().uid() == GameManager::Instance()->GetMyInfo()->GetPlayerUID()) continue;

		auto& obj = playerobj[player.userinfo().uid()];

		Vector3 pos = { player.transform().vector3().x(), player.transform().vector3().y(), player.transform().vector3().z() };
		serverPosition[index] = pos;

		Quaternion rot = { player.transform().quaternion().x(), player.transform().quaternion().y(), player.transform().quaternion().z(), player.transform().quaternion().w() };
		serverRotation[index] = rot;

		++index;
	}
}

void NetworkManager::SendPlayJump(PlayerInfo* playerinfo)
{
	Protocol::C_PLAY_JUMP packet;

	packet.mutable_playerdata()->CopyFrom(*ConvertPlayerInfoToData(playerinfo));

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvPlayJump(Protocol::PlayerData playerData)
{
	RoundManager::Instance()->GetPlayerObjs()[playerData.userinfo().uid()]->GetComponent<PlayerInfo>()->SetIsJump(true);
}

void NetworkManager::SendPlayShoot(HDData::Transform* transform, uint64 hitTargetUid /*= 0*/, Protocol::eHitLocation hitLocation /*= Protocol::eHitLocation::HIT_LOCATION_NO_HIT*/)
{
	Protocol::C_PLAY_SHOOT packet;

	auto pos = packet.mutable_transform()->mutable_vector3();
	pos->set_x(transform->GetPosition().x);
	pos->set_y(transform->GetPosition().y);
	pos->set_z(transform->GetPosition().z);

	auto rot = packet.mutable_transform()->mutable_quaternion();
	rot->set_w(transform->GetRotation().w);
	rot->set_x(transform->GetRotation().x);
	rot->set_y(transform->GetRotation().y);
	rot->set_z(transform->GetRotation().z);

	packet.set_hittargetuid(hitTargetUid);
	packet.set_hitlocation(hitLocation);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

bool NetworkManager::IsConnected()
{
	return _isConnect;
}

Protocol::PlayerData* NetworkManager::ConvertPlayerInfoToData(PlayerInfo* info)
{

	data.mutable_transform()->mutable_vector3()->set_x(info->GetTransform()->GetPosition().x);
	data.mutable_transform()->mutable_vector3()->set_y(info->GetTransform()->GetPosition().y);
	data.mutable_transform()->mutable_vector3()->set_z(info->GetTransform()->GetPosition().z);
		
	data.mutable_transform()->mutable_quaternion()->set_x(info->GetTransform()->GetRotation().x);
	data.mutable_transform()->mutable_quaternion()->set_y(info->GetTransform()->GetRotation().y);
	data.mutable_transform()->mutable_quaternion()->set_z(info->GetTransform()->GetRotation().z);
	data.mutable_transform()->mutable_quaternion()->set_w(info->GetTransform()->GetRotation().w);

	data.set_host(info->GetIsHost());

	Protocol::UserInfo* user = new Protocol::UserInfo;
	user->set_nickname(info->GetPlayerNickName());

	data.set_allocated_userinfo(user);

	return &data;
}

void NetworkManager::Interpolation(HDData::Transform* current, Vector3 serverPos, Quaternion serverRot, float intermediateValue)
{
	auto dt = API::GetDeltaTime();

	Vector3 currentPos = current->GetPosition();
	Quaternion currentRot = current->GetRotation();

	if (currentPos == serverPos && currentRot == serverRot) return;

	// 포지션 선형 보간
	Vector3 interpolatedPos = Vector3::Lerp(currentPos, serverPos, intermediateValue * dt);

	// 로테이션 구면 선형 보간
	Quaternion interpolatedRot = Quaternion::Slerp(currentRot, serverRot, intermediateValue * dt * 10);

	// 현재 Transform에 보간된 값 설정
	current->SetPosition(interpolatedPos);
	current->SetRotation(interpolatedRot);
}

