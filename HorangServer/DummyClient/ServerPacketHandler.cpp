#include "pch.h"
#include "ServerPacketHandler.h"

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
		default:
			std::cout << "정의되지 않은 에러코드 : " << pkt.errorcode() << std::endl;
			break;
	}

	return true;
}

bool Handle_S_CONNECTED(Horang::PacketSessionRef& session, Protocol::S_CONNECTED& pkt)
{
	// Todo
	// Debug

	std::cout << "자동 로그인" << std::endl;
	Protocol::C_AUTOLOGIN packet;

	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	session->Send(sendBuffer);

	return true;
}

bool Handle_S_SIGNIN_OK(Horang::PacketSessionRef& session, Protocol::S_SIGNIN_OK& pkt)
{
	std::cout << "로그인 성공! " << std::endl;
	//<< "UID : " << pkt.uid() << " NickName : " << pkt.nickname() << std::endl;

	{
		/*Protocol::C_ROOM_ENTER packet;

		packet.set_roomcode("0000");

		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
		session->Send(sendBuffer);*/
	}

	{
		Protocol::C_ROOM_CREATE packet;

		packet.set_roomname("TestRoom");
		packet.set_isteam(true);
		packet.set_map(Protocol::eRGBMap::MAP_DESERT);
		packet.set_maxplayercount(6);

		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
		session->Send(sendBuffer);
	}


	return true;
}

bool Handle_S_SIGNUP_OK(Horang::PacketSessionRef& session, Protocol::S_SIGNUP_OK& pkt)
{
	std::cout << "회원가입 성공" << std::endl;

	return true;
}

bool Handle_S_ROOM_ENTER(Horang::PacketSessionRef& session, Protocol::S_ROOM_ENTER& pkt)
{
	auto& roomInfo = pkt.roominfo();

	roomInfo.users();

	std::cout << "방 입장" << std::endl;
	std::cout << " 코드 : " << roomInfo.roomcode();
	std::cout << " 상태 : " << roomInfo.state() << std::endl;

	for (auto user : roomInfo.users())
		std::cout << "닉네임 : " << user.userinfo().nickname() << std::endl;

	return true;
}

bool Handle_S_ROOM_LEAVE(Horang::PacketSessionRef& session, Protocol::S_ROOM_LEAVE& pkt)
{

	return true;
}

bool Handle_S_ANOTHER_ENTER_ROOM(Horang::PacketSessionRef& session, Protocol::S_ANOTHER_ENTER_ROOM& pkt)
{
	auto& roomInfo = pkt.roominfo();

	std::cout << "다른 유저가 방에 들어왔습니다." << std::endl;
	std::cout << "방 번호 : " << roomInfo.roomid() << std::endl;
	std::cout << "방 코드 : " << roomInfo.roomcode() << std::endl;
	std::cout << "방 상태 : " << roomInfo.state() << std::endl;

	for (auto user : roomInfo.users())
		std::cout << "유저 UID : " << user.userinfo().uid() << " 닉네임 : " << user.userinfo().nickname() << std::endl;

	Protocol::C_ROOM_START packet;
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
	session->Send(sendBuffer);

	return true;
}

bool Handle_S_ANOTHER_LEAVE_ROOM(Horang::PacketSessionRef& session, Protocol::S_ANOTHER_LEAVE_ROOM& pkt)
{
	return true;
}

bool Handle_S_ROOM_START(Horang::PacketSessionRef& session, Protocol::S_ROOM_START& pkt)
{
	return true;
}

bool Handle_S_GAME_START(Horang::PacketSessionRef& session, Protocol::S_GAME_START& pkt)
{
	return true;
}

bool Handle_S_GAME_END(Horang::PacketSessionRef& session, Protocol::S_GAME_END& pkt)
{
	return true;
}

bool Handle_S_ROOM_CHANGE_TEAM(Horang::PacketSessionRef& session, Protocol::S_ROOM_CHANGE_TEAM& pkt)
{
	return true;
}

bool Handle_S_ROOM_KICK(Horang::PacketSessionRef& session, Protocol::S_ROOM_KICK& pkt)
{
	return true;
}

bool Handle_S_PLAY_UPDATE(Horang::PacketSessionRef& session, Protocol::S_PLAY_UPDATE& pkt)
{
	static int count = 0;

	std::cout << count++ << std::endl;
	return true;
}

bool Handle_S_ROOM_LIST(Horang::PacketSessionRef& session, Protocol::S_ROOM_LIST& pkt)
{
	// 방 리스트 출력

	std::cout << "방 목록 수 : ";
	std::cout << pkt.roominfo_size() << std::endl;

	for (auto room : pkt.roominfo())
	{
		std::cout << "이름 : " << room.roomname();
		std::cout << " - 방 코드 : " << room.roomcode();
		std::cout << " - 방 상태 : " << room.state();
		std::cout << " - " << room.currentplayercount() << "명" << std::endl;
	}

	return true;
}

bool Handle_S_PLAY_JUMP(Horang::PacketSessionRef& session, Protocol::S_PLAY_JUMP& pkt)
{
	return true;
}

bool Handle_S_PLAY_SHOOT(Horang::PacketSessionRef& session, Protocol::S_PLAY_SHOOT& pkt)
{
	return true;
}

bool Handle_S_PLAY_KILL_DEATH(Horang::PacketSessionRef& session, Protocol::S_PLAY_KILL_DEATH& pkt)
{
	return true;
}

bool Handle_S_PLAY_RESPAWN(Horang::PacketSessionRef& session, Protocol::S_PLAY_RESPAWN& pkt)
{
	return true;
}

bool Handle_S_PLAY_ROLL(Horang::PacketSessionRef& session, Protocol::S_PLAY_ROLL& pkt)
{
	return true;
}

bool Handle_S_PLAY_RELOAD(Horang::PacketSessionRef& session, Protocol::S_PLAY_RELOAD& pkt)
{
	return true;
}
