#include "pch.h"
#include <iostream>
#include "Service.h"
#include "Session.h"
#include "ThreadManager.h"
#include "BufferReader.h"
#include "ServerPacketHandler.h"
#include <fstream>

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

	int count = 64 * 4;

	Horang::ClientServiceRef service;

	std::wifstream ipAddressFile("serverIP.txt");
	std::wstring ipAddressStr = L"";
	if (ipAddressFile.is_open())
	{
		std::getline(ipAddressFile, ipAddressStr);

		service = Horang::MakeShared<Horang::ClientService>(
			Horang::NetAddress(ipAddressStr, 7776),
			Horang::MakeShared<Horang::IocpCore>(),
			Horang::MakeShared<ServerSession>, // TODO : SessionManager 등
			1
		);
	}
	else
	{
		service = Horang::MakeShared<Horang::ClientService>(
			Horang::NetAddress(L"172.16.1.13", 7776),
			Horang::MakeShared<Horang::IocpCore>(),
			Horang::MakeShared<ServerSession>, // TODO : SessionManager 등
			1
		);
	}

	ipAddressFile.close();

	//Horang::ClientServiceRef service = Horang::MakeShared<Horang::ClientService>(
	//	Horang::NetAddress(L"172.16.1.13", 7777),
	//	Horang::MakeShared<Horang::IocpCore>(),
	//	Horang::MakeShared<ServerSession>, // TODO : SessionManager 등
	//	1
	//);

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
		Protocol::C_AUTOLOGIN packet;

		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
		service->BroadCast(sendBuffer);
	}

	while (true)
	{
		int menu = 0;
		std::cout << "1. Room List" << std::endl;
		std::cout << "2. Enter Room" << std::endl;
		std::cout << "3. Create Room" << std::endl;
		std::cout << " Select : ";
		std::cin >> menu;

		::system("cls");
		if (menu == 1)
		{
			Protocol::C_ROOM_LIST_REQUEST packet;

			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
			service->BroadCast(sendBuffer);
		}
		else if (menu == 2)
		{
			Protocol::C_ROOM_ENTER packet;
			std::string roomCode = "";
			std::string password = "";

			std::cout << "Room Code : ";
			std::cin >> roomCode;
			std::cout << "Password : ";
			std::cin >> password;

			packet.set_roomcode(roomCode);
			packet.set_password(password);

			auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
			service->BroadCast(sendBuffer);
		}
		else if (menu == 3)
		{

		}
		::system("pause");
		::system("cls");

	}

	/*while (true)
	{
		Protocol::C_PLAY_UPDATE packet;
		auto playerData = packet.mutable_playerdata();
		auto transform = playerData->mutable_transform();
		Protocol::Vector3 pos;
		pos.set_x(1);
		pos.set_y(2);
		pos.set_z(3);
		Protocol::Quaternion rot;
		rot.set_x(4);
		rot.set_y(5);
		rot.set_z(6);
		rot.set_w(7);

		transform->mutable_vector3()->CopyFrom(pos);
		transform->mutable_quaternion()->CopyFrom(rot);

		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
		service->BroadCast(sendBuffer);

		::system("pause");
	}*/



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