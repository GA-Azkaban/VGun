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

#include <codecvt>
int main()
{

	{
		//
			// Todo
			// Driver와 Server IP Database name은 별도의 config파일로 분리해서 관리하기
		{
			/*ASSERT_CRASH(GDBConnectionPool->Connect(1,
				L"Driver={MySQL ODBC 8.2 Unicode Driver};\
			Server=14.35.71.148;\
			Database=VGunDB;\
			User=VGun2;\
			Password=akdgoTdj;"));*/

			ASSERT_CRASH(GDBConnectionPool->Connect(1,
				L"Driver={MySQL ODBC 8.2 Unicode Driver};\
					Server=127.0.0.1;\
					Database=VGunDB;\
					User=VGun2;\
					Password=akdgoTdj;"));
		}

		{
			//auto dbConn = GDBConnectionPool->Pop();

			//Horang::DBBind<2, 2> dbBind(*dbConn, L"SELECT uid, nickname FROM user WHERE id = (?) AND password = (?);");

			//WCHAR a[40] = L"test1";
			//WCHAR b[80] = L"test1";
			//dbBind.BindParam(0, a);
			//dbBind.BindParam(1, b);

			//int32 uid = 0;
			//WCHAR nickName[16] = L"";

			//dbBind.BindCol(0, uid);
			//dbBind.BindCol(1, nickName);

			//ASSERT_CRASH(dbBind.Execute());

			//if (dbBind.Fetch())
			//{
			//	std::wcout << "First :" << uid << " : " << nickName << std::endl;
			//}
			//else
			//{
			//	// Null
			//	std::wcout << "Fetch fail" << std::endl;
			//}

			//GDBConnectionPool->Push(dbConn);
		}

		// 회원가입
		/*for (int32 i = 0; i < 0; i++)
		{
			auto dbConn = GDBConnectionPool->Pop();

			DBBind<3, 0> dbBind(*dbConn, L"INSERT INTO user (id, password, nickname) VALUES(?, ?, ?)");

			WCHAR id[30] = L"test";
			dbBind.BindParam(0, id);

			WCHAR password[30] = L"1234";
			dbBind.BindParam(1, password);

			WCHAR nickName[30] = L"Test";
			dbBind.BindParam(2, nickName);

			TIMESTAMP_STRUCT ts = { 2023, 11, 5 };
			dbBind.BindParam(2, ts);

			ASSERT_CRASH(dbBind.Execute());

			{
				//// 기존의 바인딩 된 정보 날리기
				//dbConn->Unbind();

				//int32 gold = 100;
				//SQLLEN len = 0;
				//
				//WCHAR name[100] = L"호랑";
				//SQLLEN nameLen = 0;

				//TIMESTAMP_STRUCT ts = {};
				//ts.year = 2023;
				//ts.month = 11;
				//ts.day = 5;
				//SQLLEN tsLen = 0;

				//ASSERT_CRASH(dbConn->BindParam(1, &gold, &len));
				//ASSERT_CRASH(dbConn->BindParam(2, name, &nameLen));
				//ASSERT_CRASH(dbConn->BindParam(3, &ts, &tsLen));

				//// SQL 실행
				//ASSERT_CRASH(dbConn->Execute(L"INSERT INTO Gold (gold, name, createDate) VALUES(?, ?, ?)"));
			}

			GDBConnectionPool->Push(dbConn);
		}*/

		// Read ID
		/* {
			auto dbConn = GDBConnectionPool->Pop();

			DBBind<2, 3> dbBind(*dbConn, L"SELECT uid, id, nickname FROM user WHERE id = (?) AND password = (?)");

			WCHAR a[20] = L"test";
			WCHAR b[20] = L"1234";
			dbBind.BindParam(0, a);
			dbBind.BindParam(1, b);

			int32 uid = 0;
			WCHAR id[20] = L"";
			WCHAR nickName[16] = L"";

			dbBind.BindCol(0, uid);
			dbBind.BindCol(1, id);
			dbBind.BindCol(2, nickName);

			ASSERT_CRASH(dbBind.Execute());

			while (dbBind.Fetch())
			{
				cout << "select" << endl;
			}

			GDBConnectionPool->Push(dbConn);
		}*/

	}

	//
	GSessionManager = new GameSessionManager();
	ClientPacketHandler::Init();

	Horang::ServerServiceRef service = Horang::MakeShared<Horang::ServerService>(
		Horang::NetAddress(L"127.0.0.1", 7777),
		Horang::MakeShared<Horang::IocpCore>(),
		Horang::MakeShared<GameSession>, // TODO : SessionManager 등
		1
	);

	ASSERT_CRASH(service->Start());

	for (int i = 0; i < std::thread::hardware_concurrency(); i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			}
		);
	}

	GThreadManager->Launch([=]()
		{
			while (true)
			{
				GAuthentication.FlushJob();
				std::this_thread::yield();
			}
		}
	);

	GThreadManager->Launch([=]()
		{
			while (true)
			{
				GRoom.FlushJob();
				std::this_thread::yield();
			}
		}
	);



	GThreadManager->Join();

	return 0;
}