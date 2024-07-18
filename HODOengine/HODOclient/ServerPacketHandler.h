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
	PKT_S_CONNECTED = 1004,
	PKT_C_AUTOLOGIN = 1005,
	PKT_C_SIGNIN = 1006,
	PKT_S_SIGNIN_OK = 1007,
	PKT_C_SIGNUP = 1008,
	PKT_S_SIGNUP_OK = 1009,
	PKT_C_ROOM_CREATE = 1010,
	PKT_C_ROOM_SETTING = 1011,
	PKT_C_ROOM_ENTER = 1012,
	PKT_S_ROOM_ENTER = 1013,
	PKT_C_ROOM_LEAVE = 1014,
	PKT_S_ROOM_LEAVE = 1015,
	PKT_S_ANOTHER_ENTER_ROOM = 1016,
	PKT_S_ANOTHER_LEAVE_ROOM = 1017,
	PKT_C_ROOM_START = 1018,
	PKT_S_ROOM_START = 1019,
	PKT_S_GAME_START = 1020,
	PKT_S_GAME_END = 1021,
	PKT_C_ROOM_CHANGE_TEAM = 1022,
	PKT_S_ROOM_CHANGE_TEAM = 1023,
	PKT_C_ROOM_KICK = 1024,
	PKT_S_ROOM_KICK = 1025,
	PKT_C_PLAY_UPDATE = 1026,
	PKT_S_PLAY_UPDATE = 1027,
	PKT_C_ROOM_LIST_REQUEST = 1028,
	PKT_S_ROOM_LIST = 1029,
	PKT_C_PLAY_JUMP = 1030,
	PKT_S_PLAY_JUMP = 1031,
	PKT_C_PLAY_SHOOT = 1032,
	PKT_S_PLAY_SHOOT = 1033,
	PKT_S_PLAY_KILL_DEATH = 1034,
	PKT_S_PLAY_RESPAWN = 1035,
	PKT_C_PLAY_ROLL = 1036,
	PKT_S_PLAY_ROLL = 1037,
	PKT_C_PLAY_RELOAD = 1038,
	PKT_S_PLAY_RELOAD = 1039,
	PKT_C_ROOM_CHAT = 1040,
	PKT_S_ROOM_CHAT = 1041,
};

// Custom Handlers
bool Handle_INVALID(Horang::PacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_S_TEST(Horang::PacketSessionRef& session, Protocol::S_TEST& pkt);
bool Handle_S_ERROR(Horang::PacketSessionRef& session, Protocol::S_ERROR& pkt);
bool Handle_S_CONNECTED(Horang::PacketSessionRef& session, Protocol::S_CONNECTED& pkt);
bool Handle_S_SIGNIN_OK(Horang::PacketSessionRef& session, Protocol::S_SIGNIN_OK& pkt);
bool Handle_S_SIGNUP_OK(Horang::PacketSessionRef& session, Protocol::S_SIGNUP_OK& pkt);
bool Handle_S_ROOM_ENTER(Horang::PacketSessionRef& session, Protocol::S_ROOM_ENTER& pkt);
bool Handle_S_ROOM_LEAVE(Horang::PacketSessionRef& session, Protocol::S_ROOM_LEAVE& pkt);
bool Handle_S_ANOTHER_ENTER_ROOM(Horang::PacketSessionRef& session, Protocol::S_ANOTHER_ENTER_ROOM& pkt);
bool Handle_S_ANOTHER_LEAVE_ROOM(Horang::PacketSessionRef& session, Protocol::S_ANOTHER_LEAVE_ROOM& pkt);
bool Handle_S_ROOM_START(Horang::PacketSessionRef& session, Protocol::S_ROOM_START& pkt);
bool Handle_S_GAME_START(Horang::PacketSessionRef& session, Protocol::S_GAME_START& pkt);
bool Handle_S_GAME_END(Horang::PacketSessionRef& session, Protocol::S_GAME_END& pkt);
bool Handle_S_ROOM_CHANGE_TEAM(Horang::PacketSessionRef& session, Protocol::S_ROOM_CHANGE_TEAM& pkt);
bool Handle_S_ROOM_KICK(Horang::PacketSessionRef& session, Protocol::S_ROOM_KICK& pkt);
bool Handle_S_PLAY_UPDATE(Horang::PacketSessionRef& session, Protocol::S_PLAY_UPDATE& pkt);
bool Handle_S_ROOM_LIST(Horang::PacketSessionRef& session, Protocol::S_ROOM_LIST& pkt);
bool Handle_S_PLAY_JUMP(Horang::PacketSessionRef& session, Protocol::S_PLAY_JUMP& pkt);
bool Handle_S_PLAY_SHOOT(Horang::PacketSessionRef& session, Protocol::S_PLAY_SHOOT& pkt);
bool Handle_S_PLAY_KILL_DEATH(Horang::PacketSessionRef& session, Protocol::S_PLAY_KILL_DEATH& pkt);
bool Handle_S_PLAY_RESPAWN(Horang::PacketSessionRef& session, Protocol::S_PLAY_RESPAWN& pkt);
bool Handle_S_PLAY_ROLL(Horang::PacketSessionRef& session, Protocol::S_PLAY_ROLL& pkt);
bool Handle_S_PLAY_RELOAD(Horang::PacketSessionRef& session, Protocol::S_PLAY_RELOAD& pkt);
bool Handle_S_ROOM_CHAT(Horang::PacketSessionRef& session, Protocol::S_ROOM_CHAT& pkt);

class ServerPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[PKT_S_TEST] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_TEST>(Handle_S_TEST, session, buffer, len); };
		GPacketHandler[PKT_S_ERROR] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ERROR>(Handle_S_ERROR, session, buffer, len); };
		GPacketHandler[PKT_S_CONNECTED] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_CONNECTED>(Handle_S_CONNECTED, session, buffer, len); };
		GPacketHandler[PKT_S_SIGNIN_OK] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SIGNIN_OK>(Handle_S_SIGNIN_OK, session, buffer, len); };
		GPacketHandler[PKT_S_SIGNUP_OK] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SIGNUP_OK>(Handle_S_SIGNUP_OK, session, buffer, len); };
		GPacketHandler[PKT_S_ROOM_ENTER] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ROOM_ENTER>(Handle_S_ROOM_ENTER, session, buffer, len); };
		GPacketHandler[PKT_S_ROOM_LEAVE] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ROOM_LEAVE>(Handle_S_ROOM_LEAVE, session, buffer, len); };
		GPacketHandler[PKT_S_ANOTHER_ENTER_ROOM] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ANOTHER_ENTER_ROOM>(Handle_S_ANOTHER_ENTER_ROOM, session, buffer, len); };
		GPacketHandler[PKT_S_ANOTHER_LEAVE_ROOM] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ANOTHER_LEAVE_ROOM>(Handle_S_ANOTHER_LEAVE_ROOM, session, buffer, len); };
		GPacketHandler[PKT_S_ROOM_START] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ROOM_START>(Handle_S_ROOM_START, session, buffer, len); };
		GPacketHandler[PKT_S_GAME_START] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_GAME_START>(Handle_S_GAME_START, session, buffer, len); };
		GPacketHandler[PKT_S_GAME_END] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_GAME_END>(Handle_S_GAME_END, session, buffer, len); };
		GPacketHandler[PKT_S_ROOM_CHANGE_TEAM] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ROOM_CHANGE_TEAM>(Handle_S_ROOM_CHANGE_TEAM, session, buffer, len); };
		GPacketHandler[PKT_S_ROOM_KICK] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ROOM_KICK>(Handle_S_ROOM_KICK, session, buffer, len); };
		GPacketHandler[PKT_S_PLAY_UPDATE] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PLAY_UPDATE>(Handle_S_PLAY_UPDATE, session, buffer, len); };
		GPacketHandler[PKT_S_ROOM_LIST] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ROOM_LIST>(Handle_S_ROOM_LIST, session, buffer, len); };
		GPacketHandler[PKT_S_PLAY_JUMP] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PLAY_JUMP>(Handle_S_PLAY_JUMP, session, buffer, len); };
		GPacketHandler[PKT_S_PLAY_SHOOT] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PLAY_SHOOT>(Handle_S_PLAY_SHOOT, session, buffer, len); };
		GPacketHandler[PKT_S_PLAY_KILL_DEATH] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PLAY_KILL_DEATH>(Handle_S_PLAY_KILL_DEATH, session, buffer, len); };
		GPacketHandler[PKT_S_PLAY_RESPAWN] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PLAY_RESPAWN>(Handle_S_PLAY_RESPAWN, session, buffer, len); };
		GPacketHandler[PKT_S_PLAY_ROLL] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PLAY_ROLL>(Handle_S_PLAY_ROLL, session, buffer, len); };
		GPacketHandler[PKT_S_PLAY_RELOAD] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PLAY_RELOAD>(Handle_S_PLAY_RELOAD, session, buffer, len); };
		GPacketHandler[PKT_S_ROOM_CHAT] = [](Horang::PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ROOM_CHAT>(Handle_S_ROOM_CHAT, session, buffer, len); };
	}

	static bool HandlePacket(Horang::PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		Horang::PacketHeader* header = reinterpret_cast<Horang::PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static Horang::SendBufferRef MakeSendBuffer(Protocol::C_TEST& pkt) { return MakeSendBuffer(pkt, PKT_C_TEST); }
	static Horang::SendBufferRef MakeSendBuffer(Protocol::C_MOVE& pkt) { return MakeSendBuffer(pkt, PKT_C_MOVE); }
	static Horang::SendBufferRef MakeSendBuffer(Protocol::C_AUTOLOGIN& pkt) { return MakeSendBuffer(pkt, PKT_C_AUTOLOGIN); }
	static Horang::SendBufferRef MakeSendBuffer(Protocol::C_SIGNIN& pkt) { return MakeSendBuffer(pkt, PKT_C_SIGNIN); }
	static Horang::SendBufferRef MakeSendBuffer(Protocol::C_SIGNUP& pkt) { return MakeSendBuffer(pkt, PKT_C_SIGNUP); }
	static Horang::SendBufferRef MakeSendBuffer(Protocol::C_ROOM_CREATE& pkt) { return MakeSendBuffer(pkt, PKT_C_ROOM_CREATE); }
	static Horang::SendBufferRef MakeSendBuffer(Protocol::C_ROOM_SETTING& pkt) { return MakeSendBuffer(pkt, PKT_C_ROOM_SETTING); }
	static Horang::SendBufferRef MakeSendBuffer(Protocol::C_ROOM_ENTER& pkt) { return MakeSendBuffer(pkt, PKT_C_ROOM_ENTER); }
	static Horang::SendBufferRef MakeSendBuffer(Protocol::C_ROOM_LEAVE& pkt) { return MakeSendBuffer(pkt, PKT_C_ROOM_LEAVE); }
	static Horang::SendBufferRef MakeSendBuffer(Protocol::C_ROOM_START& pkt) { return MakeSendBuffer(pkt, PKT_C_ROOM_START); }
	static Horang::SendBufferRef MakeSendBuffer(Protocol::C_ROOM_CHANGE_TEAM& pkt) { return MakeSendBuffer(pkt, PKT_C_ROOM_CHANGE_TEAM); }
	static Horang::SendBufferRef MakeSendBuffer(Protocol::C_ROOM_KICK& pkt) { return MakeSendBuffer(pkt, PKT_C_ROOM_KICK); }
	static Horang::SendBufferRef MakeSendBuffer(Protocol::C_PLAY_UPDATE& pkt) { return MakeSendBuffer(pkt, PKT_C_PLAY_UPDATE); }
	static Horang::SendBufferRef MakeSendBuffer(Protocol::C_ROOM_LIST_REQUEST& pkt) { return MakeSendBuffer(pkt, PKT_C_ROOM_LIST_REQUEST); }
	static Horang::SendBufferRef MakeSendBuffer(Protocol::C_PLAY_JUMP& pkt) { return MakeSendBuffer(pkt, PKT_C_PLAY_JUMP); }
	static Horang::SendBufferRef MakeSendBuffer(Protocol::C_PLAY_SHOOT& pkt) { return MakeSendBuffer(pkt, PKT_C_PLAY_SHOOT); }
	static Horang::SendBufferRef MakeSendBuffer(Protocol::C_PLAY_ROLL& pkt) { return MakeSendBuffer(pkt, PKT_C_PLAY_ROLL); }
	static Horang::SendBufferRef MakeSendBuffer(Protocol::C_PLAY_RELOAD& pkt) { return MakeSendBuffer(pkt, PKT_C_PLAY_RELOAD); }
	static Horang::SendBufferRef MakeSendBuffer(Protocol::C_ROOM_CHAT& pkt) { return MakeSendBuffer(pkt, PKT_C_ROOM_CHAT); }

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