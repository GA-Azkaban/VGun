#pragma once
#include "Protocol.pb.h"

using PacketHandlerFunc = std::function<bool(Horang::PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

enum : uint16
{
	PKT_C_TEST = 1000,
	PKT_S_TEST = 1001,
	PKT_C_MOVE = 1002,
	PKT_S_ERROR = 1003,
	PKT_C_SIGNIN = 1004,
	PKT_S_SIGNIN_OK = 1005,
	PKT_C_SIGNUP = 1006,
	PKT_S_SIGNUP_OK = 1007,
};

// Custom Handlers
bool Handle_INVALID(Horang::PacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_C_TEST(Horang::PacketSessionRef& session, Protocol::C_TEST& pkt);
bool Handle_C_MOVE(Horang::PacketSessionRef& session, Protocol::C_MOVE& pkt);
bool Handle_C_SIGNIN(Horang::PacketSessionRef& session, Protocol::C_SIGNIN& pkt);
bool Handle_C_SIGNUP(Horang::PacketSessionRef& session, Protocol::C_SIGNUP& pkt);

class ClientPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[PKT_C_TEST] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_TEST>(Handle_C_TEST, session, buffer, len); };
		GPacketHandler[PKT_C_MOVE] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_MOVE>(Handle_C_MOVE, session, buffer, len); };
		GPacketHandler[PKT_C_SIGNIN] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_SIGNIN>(Handle_C_SIGNIN, session, buffer, len); };
		GPacketHandler[PKT_C_SIGNUP] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_SIGNUP>(Handle_C_SIGNUP, session, buffer, len); };
	}

	static bool HandlePacket(Horang::PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		Horang::PacketHeader* header = reinterpret_cast<Horang::PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static Horang::SendBufferRef MakeSendBuffer(Protocol::S_TEST& pkt) { return MakeSendBuffer(pkt, PKT_S_TEST); }
	static Horang::SendBufferRef MakeSendBuffer(Protocol::S_ERROR& pkt) { return MakeSendBuffer(pkt, PKT_S_ERROR); }
	static Horang::SendBufferRef MakeSendBuffer(Protocol::S_SIGNIN_OK& pkt) { return MakeSendBuffer(pkt, PKT_S_SIGNIN_OK); }
	static Horang::SendBufferRef MakeSendBuffer(Protocol::S_SIGNUP_OK& pkt) { return MakeSendBuffer(pkt, PKT_S_SIGNUP_OK); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, Horang::PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(Horang::PacketHeader), len - sizeof(Horang::PacketHeader)) == false)
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static Horang::SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(Horang::PacketHeader);

		Horang::SendBufferRef sendBuffer = GSendBufferManager->Open(packetSize);
		Horang::PacketHeader* header = reinterpret_cast<Horang::PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		ASSERT_CRASH(pkt.SerializeToArray(&header[1], dataSize));
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};