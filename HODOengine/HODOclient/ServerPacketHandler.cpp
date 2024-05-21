#include "pch.h"
#include "ServerPacketHandler.h"

#include "../HODOengine/ObjectSystem.h"
#include "../HODOengine/GameObject.h"
#include "../HODOengine/Component.h"

#include "NetworkManager.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

/*
	ServerPacketHandler
*/

bool Handle_INVALID(Horang::PacketSessionRef& session, BYTE* buffer, int32 len)
{
	Horang::PacketHeader* header = reinterpret_cast<Horang::PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_S_TEST(Horang::PacketSessionRef& session, Protocol::S_TEST& pkt)
{
	Protocol::C_TEST packet;

	//cout << pkt.num() << endl;

	packet.set_num(pkt.num() + 1);

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	session->Send(sendBuffer);

	return true;
}

bool Handle_S_ERROR(Horang::PacketSessionRef& session, Protocol::S_ERROR& pkt)
{
	//cout << pkt.errorcode() << endl;

	switch (pkt.errorcode())
	{
		case 1001:
			std::cout << "로그인 실패" << std::endl;
			break;
		case 1004:
			std::cout << "회원가입 실패" << std::endl;
			break;
		case 1002:
			std::cout << "ID 중복" << std::endl;
			break;
		case 1003:
			std::cout << "닉네임 중복" << std::endl;
			break;
		default:
			std::cout << "정의되지 않은 에러코드 : " << pkt.errorcode() << std::endl;
			break;
	}

	NetworkManager::Instance().RecvFail(pkt.errorcode());

	return true;
}

bool Handle_S_CONNECTED(Horang::PacketSessionRef& session, Protocol::S_CONNECTED& pkt)
{
	NetworkManager::Instance().Connected();

	return true;
}

bool Handle_S_SIGNIN_OK(Horang::PacketSessionRef& session, Protocol::S_SIGNIN_OK& pkt)
{
	std::cout << "Login " << "UID : " << pkt.uid() << " NickName : " << pkt.nickname() << std::endl;

	NetworkManager::Instance().RecvLogin(pkt.uid(), pkt.nickname());

	return true;
}

bool Handle_S_SIGNUP_OK(Horang::PacketSessionRef& session, Protocol::S_SIGNUP_OK& pkt)
{
	std::cout << "CreateAccount" << std::endl;

	NetworkManager::Instance().RecvCreateAccount();

	return true;
}

bool Handle_S_ROOM_ENTER(Horang::PacketSessionRef& session, Protocol::S_ROOM_ENTER& pkt)
{
	std::cout << "Room Enter" << std::endl;

	NetworkManager::Instance().RecvRoomEnter(pkt.roominfo());

	return true;
}

bool Handle_S_ROOM_LEAVE(Horang::PacketSessionRef& session, Protocol::S_ROOM_LEAVE& pkt)
{
	std::cout << "Room Leave" << std::endl;

	NetworkManager::Instance().RecvRoomLeave(pkt.roominfo());

	return true;
}

bool Handle_S_ANOTHER_ENTER_ROOM(Horang::PacketSessionRef& session, Protocol::S_ANOTHER_ENTER_ROOM& pkt)
{
	std::cout << "Another Enter" << std::endl;

	NetworkManager::Instance().RecvAnotherPlayerEnter(pkt.roominfo());

	return true;
}

bool Handle_S_ANOTHER_LEAVE_ROOM(Horang::PacketSessionRef& session, Protocol::S_ANOTHER_LEAVE_ROOM& pkt)
{
	std::cout << "Another Leave" << std::endl;

	NetworkManager::Instance().RecvAnotherPlayerLeave(pkt.roominfo());

	return true;
}

bool Handle_S_ROOM_START(Horang::PacketSessionRef& session, Protocol::S_ROOM_START& pkt)
{
	NetworkManager::Instance().RecvGameStart();

	return true;
}

bool Handle_S_ROOM_CHANGE_TEAM(Horang::PacketSessionRef& session, Protocol::S_ROOM_CHANGE_TEAM& pkt)
{
	NetworkManager::Instance().RecvChangeTeamColor(pkt.roominfo());

	return true;
}

bool Handle_S_ROOM_KICK(Horang::PacketSessionRef& session, Protocol::S_ROOM_KICK& pkt)
{
	NetworkManager::Instance().RecvKickPlayer(pkt.roominfo());
	
	return true;
}

bool Handle_S_PLAY_UPDATE(Horang::PacketSessionRef& session, Protocol::S_PLAY_UPDATE& pkt)
{
	NetworkManager::Instance().RecvPlayUpdate(pkt);

	return true;
}

bool Handle_S_ROOM_LIST(Horang::PacketSessionRef& session, Protocol::S_ROOM_LIST& pkt)
{
	NetworkManager::Instance().RecvRoomList(pkt);

	return true;
}
