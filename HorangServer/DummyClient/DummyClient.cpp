#include "pch.h"
#include <iostream>
#include "Service.h"
#include "Session.h"
#include "ThreadManager.h"
#include "BufferReader.h"
#include "ServerPacketHandler.h"

using namespace std::chrono_literals;

class ServerSession : public Horang::PacketSession
{
public:
	~ServerSession()
	{
		std::cout << "~ServerSession" << std::endl;
	}

	virtual void OnConnected() override
	{
		std::cout << "Connected To Server" << std::endl;
	}

	virtual void OnRecvPacket(BYTE* buffer, int32 len) override
	{
		Horang::PacketSessionRef session = GetPacketSessionRef();
		Horang::PacketHeader* header = reinterpret_cast<Horang::PacketHeader*>(buffer);

		ServerPacketHandler::HandlePacket(session, buffer, len);
	}

	virtual void OnSend(int32 len) override
	{
		//cout << "OnSend Len = " << len << endl;
	}

	virtual void OnDisconnected() override
	{
		std::cout << "Disconnected" << std::endl;
	}
};

int main()
{
	ServerPacketHandler::Init();


	int count = 64*4;

	Horang::ClientServiceRef service = Horang::MakeShared<Horang::ClientService>(
		Horang::NetAddress(L"127.0.0.1", 7777),
		Horang::MakeShared<Horang::IocpCore>(),
		Horang::MakeShared<ServerSession>, // TODO : SessionManager 등
		1
	);

	ASSERT_CRASH(service->Start());

	std::this_thread::sleep_for(1s);

	for (int32 i = 0; i < 1; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	std::this_thread::sleep_for(1s);

	{
		Protocol::C_SIGNUP signUpPkt;

		signUpPkt.set_id(u8"test1");
		signUpPkt.set_password(u8"test1");
		signUpPkt.set_nickname(u8"test1");

		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(signUpPkt);
		service->BroadCast(sendBuffer);
	}

	{
		Protocol::C_SIGNIN signInPkt;

		signInPkt.set_id(u8"test1");
		signInPkt.set_password(u8"test1");

		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(signInPkt);
		service->BroadCast(sendBuffer);
	}

	/*Protocol::C_CHAT chatPkt;
	chatPkt.set_msg(u8"Hello World");
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(chatPkt);
	while (true)
	{
		service->BroadCast(sendBuffer);

		this_thread::sleep_for(1s);
	}*/
	

	/*for (int32 i = 0; i < 4; i++)
	{
		GThreadManager->Launch([=]()
			{
				auto startTime = std::chrono::steady_clock::now();

				int second = 0;

				while (true)
				{
					auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime);;

					if (deltaTime.count() >= 1000 / 60)
					{
						Protocol::C_TEST packet;
						packet.set_num(0);
						for (int i = 0; i < count / 4; i++)
						{
							auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
							service->BroadCast(sendBuffer);
						}

						// 시작 시간 갱신
						startTime = std::chrono::steady_clock::now();
					}

					std::this_thread::sleep_for(1ms);
				}
			});
	}*/



	GThreadManager->Join();

	return 0;
}