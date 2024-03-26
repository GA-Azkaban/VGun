#include "pch.h"
#include "NetworkManager.h"

#include "Service.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"

#include "GameManager.h"
#include "LobbyManager.h"

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

void NetworkManager::RecvLogin(int32 uid, std::string nickName)
{
	// remove pyramid
	// 로그인이 성공했을때 처리
	LobbyManager::Instance().LoginSucess(uid, nickName);
}

bool NetworkManager::IsConnected()
{
	return _isConnect;
}
