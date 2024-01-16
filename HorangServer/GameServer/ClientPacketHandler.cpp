#include "pch.h"
#include "ClientPacketHandler.h"
#include "Player.h"
#include "Room.h"
#include "GameSession.h"
#include "DBConnectionPool.h"
#include "DBBind.h"
#include "DBConnector.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

// 직접 만들기
bool Handle_INVALID(Horang::PacketSessionRef& session, BYTE* buffer, int32 len)
{
	Horang::PacketHeader* header = reinterpret_cast<Horang::PacketHeader*>(buffer);

	return false;
}

bool Handle_C_TEST(Horang::PacketSessionRef& session, Protocol::C_TEST& pkt)
{
	Protocol::S_TEST patket;
	patket.set_num(pkt.num() + 1);

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(patket);
	// session->Send(sendBuffer);

	return true;
}

bool Handle_C_MOVE(Horang::PacketSessionRef& session, Protocol::C_MOVE& pkt)
{
	return true;
}

bool Handle_C_SIGNIN(Horang::PacketSessionRef& session, Protocol::C_SIGNIN& pkt)
{
	if (pkt.id().length() > 40 || pkt.password().length() > 80)
		return false;

	auto dbConn = GDBConnectionPool->Pop();

	DB::SignIn signIn(*dbConn);

	WCHAR id[40] = L"";
	WCHAR password[80] = L"";

	signIn.In_Id(id, pkt.id());
	signIn.In_Password(password, pkt.password());

	int32 uid = 0;
	WCHAR nickName[16] = L"";

	signIn.Out_Uid(uid);
	signIn.Out_NickName(nickName);

	ASSERT_CRASH(signIn.Execute());

	if (signIn.Fetch() == true)
	{
		std::wcout << uid << " : " << nickName << std::endl;

		// 성공 동작
		Protocol::S_SIGNIN_OK packet;
		packet.set_uid(uid);
		packet.set_nickname(boost::locale::conv::utf_to_utf<char>(std::wstring(nickName)));

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		session->Send(sendBuffer);
	}
	else
	{
		// 실패 동작
		Protocol::S_ERROR packet;
		packet.set_errorcode(ErrorCode::SIGNIN_FAIL);

		// Todo Log

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		session->Send(sendBuffer);
	}

	GDBConnectionPool->Push(dbConn);

	return true;
}

bool Handle_C_SIGNUP(Horang::PacketSessionRef& session, Protocol::C_SIGNUP& pkt)
{
	if (pkt.id().length() > 40 ||
		pkt.password().length() > 80 ||
		pkt.nickname().length() > 16)
		return false;

	auto dbConn = GDBConnectionPool->Pop();

	DB::SignUp signUp(*dbConn);

	WCHAR id[40] = L"";
	WCHAR password[80] = L"";
	WCHAR nickName[16] = L"";

	signUp.In_Id(id, pkt.id());
	signUp.In_Password(password, pkt.password());
	signUp.In_NickName(nickName, pkt.nickname());

	if (signUp.Execute() && signUp.Fetch())
	{
		Protocol::S_SIGNUP_OK packet;

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		session->Send(sendBuffer);
	}
	else
	{
		Protocol::S_ERROR packet;

		packet.set_errorcode(ErrorCode::SIGNUP_FAIL);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		session->Send(sendBuffer);
	}

	GDBConnectionPool->Push(dbConn);

	return true;
}
