#include "pch.h"
#include "ClientPacketHandler.h"
#include "Player.h"
#include "JobQueue.h"
#include "Room.h"
#include "GameSession.h"
#include "DBConnectionPool.h"
#include "DBBind.h"
#include "DBConnector.h"
#include "AuthenticationManager.h"
#include "RoomManager.h"

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
	/*
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

	*/
	GAuthentication.Push(Horang::MakeShared<SignInJob>(session, pkt.id(), pkt.password()));

	return true;
}

bool Handle_C_SIGNUP(Horang::PacketSessionRef& session, Protocol::C_SIGNUP& pkt)
{
	/*
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

	int32 result = 0;
	signUp.Out_Result(result);

	ASSERT_CRASH(signUp.Execute());

	if (signUp.Fetch())
	{
		if (result == 1)
		{
			Protocol::S_SIGNUP_OK packet;

			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
			session->Send(sendBuffer);
		}
		else
		{
			Protocol::S_ERROR packet;
			// Todo Log

			packet.set_errorcode(result);

			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
			session->Send(sendBuffer);
		}
	}
	else
	{
		Protocol::S_ERROR packet;

		packet.set_errorcode(ErrorCode::SIGNUP_FAIL);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		session->Send(sendBuffer);
	}

	GDBConnectionPool->Push(dbConn);
	*/

	GAuthentication.Push(Horang::MakeShared<SignUpJob>(session, pkt.id(), pkt.password(), pkt.nickname()));

	return true;
}

bool Handle_C_ROOM_CREATE(Horang::PacketSessionRef& session, Protocol::C_ROOM_CREATE& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	GRoomManager.Push(Horang::MakeShared<CreateRoomJob>(gameSession->_player, pkt.roomname(),
		pkt.password(),
		pkt.maxplayercount(),
		pkt.isprivate(),
		pkt.isteam()
	));

	return true;
}

bool Handle_C_ROOM_ENTER(Horang::PacketSessionRef& session, Protocol::C_ROOM_ENTER& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	GRoomManager.Push(Horang::MakeShared<EnterRoomJob>(gameSession->_player, stoi(pkt.roomcode())));

	return true;
}

bool Handle_C_ROOM_LEAVE(Horang::PacketSessionRef& session, Protocol::C_ROOM_LEAVE& pkt)
{
	// Todo
	auto gameSession = static_pointer_cast<GameSession>(session);

	auto room = gameSession->_room.lock();
	if (room == nullptr)
		return false;

	room->Push(Horang::MakeShared<LeaveJob>(room, static_pointer_cast<GameSession>(session)->_player));

	return true;
}

bool Handle_C_ROOM_START(Horang::PacketSessionRef& session, Protocol::C_ROOM_START& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	auto room = gameSession->_room.lock();
	if (room == nullptr)
		return false;

	room->Push(Horang::MakeShared<GameStartJob>(room, gameSession->_player));

	return true;
}

bool Handle_C_PLAY_UPDATE(Horang::PacketSessionRef& session, Protocol::C_PLAY_UPDATE& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	auto room = gameSession->_room.lock();
	if (room == nullptr)
		return false;

	room->Push(Horang::MakeShared<ClientUpdateJob>(gameSession->_room, gameSession->_player, pkt));

	return true;
}

bool Handle_C_ROOM_LIST_REQUEST(Horang::PacketSessionRef& session, Protocol::C_ROOM_LIST_REQUEST& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	GRoomManager.Push(Horang::MakeShared<SendRoomListJob>(gameSession->_player));

	return true;
}
