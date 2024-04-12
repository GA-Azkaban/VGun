#include "pch.h"
#include "NetworkManager.h"

#include "Service.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"

#include "GameManager.h"

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
		Horang::NetAddress(L"127.0.0.1", 7777),
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
	switch (errorCode)
	{
		case LOGIN_FAIL:

			break;
		case ID_DUPLICATION:
			break;

		case NICKNAME_DUPLICATION:
			break;

		case SIGNUP_FAIL:
			break;

		default:
			std::cout << "Unknown Error Code" << std::endl;
			break;
	}

}

void NetworkManager::RecvLogin(int32 uid, std::string nickName)
{
	// remove pyramid
	// 로그인이 성공했을때 처리

}

void NetworkManager::RecvCreateAccount()
{
	// 회원가입 성공 처리
}

void NetworkManager::SendRoomEnter(std::string roomCode)
{
	Protocol::C_ROOM_ENTER packet;
	packet.set_roomcode(roomCode);

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
