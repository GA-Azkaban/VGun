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
	NetworkManager::Instance().RecvRoomStart(pkt.roominfo(), pkt.gamerule(), pkt.spawnpointindex());

	return true;
}

bool Handle_S_GAME_START(Horang::PacketSessionRef& session, Protocol::S_GAME_START& pkt)
{
	NetworkManager::Instance().RecvGameStart();
	return true;
}

bool Handle_S_GAME_END(Horang::PacketSessionRef& session, Protocol::S_GAME_END& pkt)
{
	NetworkManager::Instance().RecvGameEnd(pkt.roominfo());
	return true;
}

bool Handle_S_ROOM_CHANGE_TEAM(Horang::PacketSessionRef& session, Protocol::S_ROOM_CHANGE_TEAM& pkt)
{
	//NetworkManager::Instance().RecvChangeTeamColor(pkt.roominfo());

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

bool Handle_S_PLAY_JUMP(Horang::PacketSessionRef& session, Protocol::S_PLAY_JUMP& pkt)
{
	NetworkManager::Instance().RecvPlayJump(pkt.playerdata());
	return true;
}

bool Handle_S_PLAY_SHOOT(Horang::PacketSessionRef& session, Protocol::S_PLAY_SHOOT& pkt)
{
	if (pkt.has_hitplayer() && pkt.has_hitlocation())
	{
		NetworkManager::Instance().RecvPlayShoot(pkt.shootplayer(), pkt.hitplayer(), pkt.hitlocation());
		return true;
	}

	NetworkManager::Instance().RecvPlayShoot(pkt.shootplayer());
	return true;
}

bool Handle_S_PLAY_KILL_DEATH(Horang::PacketSessionRef& session, Protocol::S_PLAY_KILL_DEATH& pkt)
{
	NetworkManager::Instance().RecvPlayKillDeath(pkt.deathplayer(), pkt.killplayer());

	return true;
}

bool Handle_S_PLAY_RESPAWN(Horang::PacketSessionRef& session, Protocol::S_PLAY_RESPAWN& pkt)
{
	NetworkManager::Instance().RecvPlayRespawn(pkt.playerdata(), pkt.spawnpointindex());

	return true;
}

bool Handle_S_PLAY_ROLL(Horang::PacketSessionRef& session, Protocol::S_PLAY_ROLL& pkt)
{
	NetworkManager::Instance().RecvPlayRoll(pkt.playerdata());

	return true;
}

bool Handle_S_PLAY_RELOAD(Horang::PacketSessionRef& session, Protocol::S_PLAY_RELOAD& pkt)
{
	NetworkManager::Instance().RecvPlayReload(pkt.playerdata());

	return true;
}

bool Handle_S_ROOM_CHAT(Horang::PacketSessionRef& session, Protocol::S_ROOM_CHAT& pkt)
{
	NetworkManager::Instance().RecvRoomChat(pkt.nickname(), pkt.chat());

	return true;
}

bool Handle_S_SIGNOUT_OK(Horang::PacketSessionRef& session, Protocol::S_SIGNOUT_OK& pkt)
{
	// Todo 로그아웃 성공시 오는데 쓸 일 있을까
	NetworkManager::Instance().RecvLogout();

	return true;
}
