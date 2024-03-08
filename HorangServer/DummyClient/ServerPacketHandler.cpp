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


	return true;
}

bool Handle_S_SIGNUP_OK(Horang::PacketSessionRef& session, Protocol::S_SIGNUP_OK& pkt)
{
	std::cout << "회원가입 성공!" << std::endl;

	return true;
}
