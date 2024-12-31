#include "pch.h"
#include "ServerSession.h"
#include "ServerPacketHandler.h"
#include "NetworkManager.h"

void ServerSession::OnConnected()
{
	
}

void ServerSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	Horang::PacketSessionRef session = GetPacketSessionRef();
	Horang::PacketHeader* header = reinterpret_cast<Horang::PacketHeader*>(buffer);

	ServerPacketHandler::HandlePacket(session, buffer, len);
}

void ServerSession::OnSend(int32 len)
{

}

void ServerSession::OnDisconnected()
{
	NetworkManager::Instance().Disconnected();
}
