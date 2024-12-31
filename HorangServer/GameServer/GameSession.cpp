#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "ClientPacketHandler.h"
#include "ErrorCode.h"
#include "JobQueue.h"
#include "Room.h"
#include "AuthenticationManager.h"
#include "Player.h"
#include "JobTimer.h"

void GameSession::OnConnected()
{
	GSessionManager->Add(static_pointer_cast<GameSession>(shared_from_this()));

	Protocol::S_CONNECTED packet;

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
	this->Send(sendBuffer);
}

void GameSession::OnDisconnected()
{
	if (this->_player)
	{
		if (auto room = _room.lock())
		{
			room->Push(Horang::MakeShared<LeaveJob>(room, _player,_player->uid));
		}

		// 1초뒤에 DisconnectJob 넣어주기
		//GAuthentication->Push(Horang::MakeShared<DisconnectJob>(_player->uid));
		auto job = Horang::MakeShared<DisconnectJob>(_player->uid);
		GJobTimer->Reserve(1000, GAuthentication->weak_from_this(), job);
	}

	GSessionManager->Remove(static_pointer_cast<GameSession>(shared_from_this()));

	_player = nullptr;
}

void GameSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	Horang::PacketSessionRef session = GetPacketSessionRef();
	Horang::PacketHeader* header = reinterpret_cast<Horang::PacketHeader*>(buffer);

	// TODO 패킷 ID 대역 체크 - 클라 서버 or DB
	ClientPacketHandler::HandlePacket(session, buffer, len);
}

void GameSession::OnSend(int32 len)
{
	//cout << "OnSend Len : " << len << endl;
}

void GameSession::SendError(ErrorCode errorCode)
{
	Protocol::S_ERROR packet;
	packet.set_errorcode(static_cast<int32>(errorCode));

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
	this->Send(sendBuffer);
}