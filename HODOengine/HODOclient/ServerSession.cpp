#include "pch.h"
#include "ServerSession.h"
#include "ServerPacketHandler.h"
#include "NetworkManager.h"

void ServerSession::OnConnected()
{
	NetworkManager::Instance().SetConnect(true);

	// Todo : Remove this code
#if _DEBUG
	FILE* pFile = nullptr;

	if (AllocConsole())
		freopen_s(&pFile, "CONOUT$", "w", stdout);
	else
		ASSERT_CRASH(false);

	std::cout << "Connect" << std::endl;
#endif
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
	NetworkManager::Instance().SetConnect(false);
}
