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

	return true;
}

bool Handle_S_SIGNIN_OK(Horang::PacketSessionRef& session, Protocol::S_SIGNIN_OK& pkt)
{
	std::cout << "로그인 성공! " << std::endl;
	//<< "UID : " << pkt.uid() << " NickName : " << pkt.nickname() << std::endl;

	{
		Protocol::C_ROOM_ENTER packet;

		packet.set_roomcode("0000");

		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
		session->Send(sendBuffer);
	}

	return true;
}

bool Handle_S_SIGNUP_OK(Horang::PacketSessionRef& session, Protocol::S_SIGNUP_OK& pkt)
{
	std::cout << "회원가입 성공!" << std::endl;

	return true;
}

bool Handle_S_ROOM_ENTER(Horang::PacketSessionRef& session, Protocol::S_ROOM_ENTER& pkt)
{

	auto& roomInfo = pkt.roominfo();

	roomInfo.users();

	std::cout << "방 입장 성공!" << std::endl;
	std::cout << "방 번호 : " << roomInfo.roomid() << std::endl;
	std::cout << "방 코드 : " << roomInfo.roomcode() << std::endl;
	std::cout << "방 상태 : " << roomInfo.state() << std::endl;

	for (auto user : roomInfo.users())
		std::cout << "유저 ID : " << user.id() << " 닉네임 : " << user.nickname() << std::endl;

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
		std::cout << "유저 ID : " << user.id() << " 닉네임 : " << user.nickname() << std::endl;


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

bool Handle_S_PLAY_UPDATE(Horang::PacketSessionRef& session, Protocol::S_PLAY_UPDATE& pkt)
{
	static int count = 0;

	std::cout << count++ << std::endl;
	return true;
}
