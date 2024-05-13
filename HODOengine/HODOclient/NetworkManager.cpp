#include "pch.h"
#include "NetworkManager.h"

#include "Service.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"

#include "GameManager.h"
#include "LobbyManager.h"
#include "MenuManager.h"
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
	Start();
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

void NetworkManager::SendLogin(std::string id, std::string password)
{
	Protocol::C_SIGNIN packet;
	packet.set_id(id);
	packet.set_password(password);

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
}

void NetworkManager::RecvCreateAccount()
{
	// 회원가입 성공 처리
}

void NetworkManager::SendRoomListRequest()
{
	Protocol::C_ROOM_LIST_REQUEST packet;

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvRoomList(Protocol::S_ROOM_LIST roomList)
{
	// vector처럼 사용가능
	for (auto room : roomList.roominfo())
	{
		auto id = room.roomid();
		auto name = room.roomname();
		auto maxPlayerCount = room.maxplayercount();
		auto currentPlayerCount = room.currentplayercount();

		auto isPrivate = room.isprivate();
		auto isTeam = room.isteam();
	}

	// Todo 방 리스트 처리
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

}

void NetworkManager::RecvRoomLeave(Protocol::RoomInfo roomInfo)
{
	// Todo 

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

void NetworkManager::SetRoom()
{
	Protocol::C_ROOM_CREATE packet;

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvAnotherPlayerEnter(Protocol::RoomInfo roomInfo)
{

}

void NetworkManager::RecvAnotherPlayerLeave(Protocol::RoomInfo roomInfo)
{

}

void NetworkManager::SendGameStart()
{
	Protocol::C_ROOM_START packet;

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	this->_service->BroadCast(sendBuffer);
}

void NetworkManager::RecvGameStart()
{
	// Todo 게임 시작 수신
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

void NetworkManager::SetConnect(bool isConnect)
{
	this->_isConnect = isConnect;
}
