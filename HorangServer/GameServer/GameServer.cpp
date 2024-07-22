#include "pch.h"
#include <iostream>

#include "ThreadManager.h"

#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ClientPacketHandler.h"
#include "tchar.h"
#include "Protocol.pb.h"
#include "DBConnectionPool.h"
#include "DBBind.h"
#include "DBConnectionRef.h"
#include "Room.h"
#include "AuthenticationManager.h"
#include "RoomManager.h"
#include "JobTimer.h"
#include "Player.h"

#include "Log.h"

class TestClass : public Horang::JobQueue
{
public:
	void Test()
	{
		std::cout << "TestJob" << std::endl;
	}
};

class TestJob : public Horang::IJob
{
public:
	TestJob(std::shared_ptr<TestClass> testClass)
		: mTestClass(testClass)
	{
	}

	void Execute() override
	{
		std::cout << "Execute" << std::endl;
		mTestClass->Test();
	}

	std::shared_ptr<TestClass> mTestClass;
};

bool g_useDB = false;
int g_gameTime = 120;

int main(int argc, char* argv[])
{
	// 172.16.1.13
	//std::wstring ip = L"127.0.0.1";
	std::wstring ip = L"172.16.1.13";
	int port = 7777;
	if (ip == L"172.16.1.13")
		g_useDB = true;

	if (argc >= 2)
	{
		std::string str(argv[1]);
		ip = std::wstring(str.begin(), str.end());
	}
	if (argc >= 3)
		port = atoi(argv[2]);
	if (argc >= 4)
		if (strcmp(argv[3], "true") == 0)
			g_useDB = true;
	if (argc >= 5)
		g_gameTime = atoi(argv[4]);

	if (g_useDB)
	{
		// Todo Driver와 Server IP Database name은 별도의 config파일로 분리해서 관리하기
		ASSERT_CRASH
		(
			GDBConnectionPool->Connect
			(
				1,
				L"Driver={MySQL ODBC 8.2 Unicode Driver};\
					Server=127.0.0.1;\
					Database=VGunDB;\
					User=VGun2;\
					Password=akdgoTdj;"
			)
		);
	}

	//
	GSessionManager = new GameSessionManager();
	ClientPacketHandler::Init();
	GRoomManager = Horang::MakeShared<RoomManager>();
	GAuthentication = Horang::MakeShared<AuthenticationManager>();

	std::wcout << L"Server IP : " << ip.c_str() << " : " << port << " " << g_useDB << std::endl;
	std::wcout << L"GameTime : " << g_gameTime << " seconds" << std::endl;

	Horang::ServerServiceRef service = Horang::MakeShared<Horang::ServerService>(
		Horang::NetAddress(ip.c_str(), port),
		Horang::MakeShared<Horang::IocpCore>(),
		Horang::MakeShared<GameSession>, // TODO : SessionManager 등
		1
	);

	ASSERT_CRASH(service->Start());

	for (int i = 0; i < std::thread::hardware_concurrency() / 2; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					LEndTickCount = ::GetTickCount64() + 64;

					service->GetIocpCore()->Dispatch(10);

					Horang::ThreadManager::DistributeReservedJobs();

					Horang::ThreadManager::DoGlobalQueueWork();
				}
			}
		);
	}


	GRoomManager->Initialize();

	/*
	GRoomManager->CreateRoom("TestRoom1", "", 6, false, false,1);
	GRoomManager->CreateRoom("TestRoom2", "123", 5, true, false,2);
	GRoomManager->CreateRoom("TestRoom3", "", 6, false, true,3);

	auto testClass = Horang::MakeShared<TestClass>();
	GJobTimer->Reserve(100, testClass->weak_from_this(), Horang::MakeShared<TestJob>(testClass));
	GJobTimer->Reserve(300, testClass->weak_from_this(), Horang::MakeShared<TestJob>(testClass));
	*/

	GThreadManager->Join();

	return 0;
}