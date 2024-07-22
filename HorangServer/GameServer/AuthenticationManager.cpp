#include "pch.h"
#include "AuthenticationManager.h"

#include "DBConnectionPool.h"
#include "DBConnector.h"

#include "ClientPacketHandler.h"
#include "Player.h"
#include "GameSession.h"
#include "Log.h"

extern bool g_useDB;

using Horang::LogBuffer;

AuthenticationManagerRef GAuthentication;

AuthenticationManager::AuthenticationManager()
{

}

bool AuthenticationManager::SignIn(Horang::PacketSessionRef session, std::string id, std::string password)
{
	LogBuffer log("SignIn");

	ASSERT_CRASH(g_useDB);

	{
		// 로그인 되어있는지 검사
		auto gameSession = static_pointer_cast<GameSession>(session);

		if (gameSession->_player != nullptr)
		{
			Protocol::S_ERROR packet;
			packet.set_errorcode(static_cast<int32>(ErrorCode::SIGNIN_ALREADY));

			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
			session->Send(sendBuffer);

			log << "Already Login";

			return false;
		}
	}

	if (id.length() > 40 || password.length() > 80)
	{
		log << "Length Error";
		return false;
	}

	auto dbConn = GDBConnectionPool->Pop();

	DB::SignIn signIn(*dbConn);

	WCHAR wid[40] = L"";
	WCHAR wpassword[80] = L"";

	signIn.In_Id(wid, id);
	signIn.In_Password(wpassword, password);

	int32 uid = 0;
	WCHAR nickName[16] = L"";

	signIn.Out_Uid(uid);
	signIn.Out_NickName(nickName);

	ASSERT_CRASH(signIn.Execute());

	if (signIn.Fetch() == true)
	{
		std::wcout << uid << " : " << nickName << std::endl;
		log << "SignIn Success";

		GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

		if (this->isConnect(uid))
		{
			// 이미 로그인 중
			Protocol::S_ERROR packet;
			packet.set_errorcode(static_cast<int32>(ErrorCode::SIGNIN_DUPLICATE));

			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
			session->Send(sendBuffer);

			GDBConnectionPool->Push(dbConn);

			log << "Duplicate Login";

			return false;
		}
		else
		{
			// 로그인 성공
			PlayerRef player = Horang::MakeShared<Player>();
			player->uid = uid;
			player->id = id;
			std::wstring wstr{ nickName };
			player->nickname = std::string(wstr.begin(), wstr.end());
			player->ownerGameSession = gameSession;

			gameSession->_player = player;

			this->Connect(gameSession);

			// 성공 동작
			Protocol::S_SIGNIN_OK packet;
			packet.set_uid(uid);
			packet.set_nickname(boost::locale::conv::utf_to_utf<char>(std::wstring(nickName)));

			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
			session->Send(sendBuffer);

			GDBConnectionPool->Push(dbConn);

			log << "Success";

			return true;
		}
	}
	else
	{
		// 실패 동작
		Protocol::S_ERROR packet;
		packet.set_errorcode(static_cast<int32>(ErrorCode::SIGNIN_FAIL));

		// Todo Log

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		session->Send(sendBuffer);

		GDBConnectionPool->Push(dbConn);

		log << "Fail";

		return false;
	}

	GDBConnectionPool->Push(dbConn);
	return true;
}

bool AuthenticationManager::SignUp(Horang::PacketSessionRef session, std::string id, std::string password, std::string nickname)
{
	LogBuffer log("SignUp");

	ASSERT_CRASH(g_useDB);

	if (id.length() > 40 ||
		password.length() > 80 ||
		nickname.length() > 16)
	{
		log << "Length Error";
		return false;
	}

	auto dbConn = GDBConnectionPool->Pop();

	DB::SignUp signUp(*dbConn);

	WCHAR wid[40] = L"";
	WCHAR wpassword[80] = L"";
	WCHAR wnickName[16] = L"";

	signUp.In_Id(wid, id);
	signUp.In_Password(wpassword, password);
	signUp.In_NickName(wnickName, nickname);

	int32 result = 0;
	signUp.Out_Result(result);

	ASSERT_CRASH(signUp.Execute());

	if (signUp.Fetch())
	{
		log << "SignUp Success";
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
		packet.set_errorcode(static_cast<int32>(ErrorCode::SIGNUP_FAIL));

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		session->Send(sendBuffer);

		log << "Fail";
	}

	GDBConnectionPool->Push(dbConn);
	return true;
}

bool AuthenticationManager::Connect(GameSessionRef session)
{
	if (session->_player->uid == 0)
		return false;

	_activeAccount[session->_player->uid] = session;
	return true;
}

bool AuthenticationManager::Disconnect(int32 uid)
{
	if (uid == 0)
		return false;

	_activeAccount.erase(uid);
	return true;
}

void AuthenticationManager::PrintActiveAccount()
{
	std::cout << "Active Account : " << _activeAccount.size() << std::endl;

	for (auto& [uid, session] : _activeAccount)
	{
		std::cout << "UID : " << uid << " ID : " << session->_player->id << " Nickname : " << session->_player->nickname << std::endl;
	}
}

void AuthenticationManager::AutoLogin(Horang::PacketSessionRef session, std::string userNickName)
{
	LogBuffer log("AutoLogin");

	static int32 dummyId = 1;

	// 디비 미사용
	if (!g_useDB)
	{
		log << "Not Use DB";

		std::string id = "Guest" + std::to_string(dummyId);
		std::string nickName = userNickName.length() > 0 ? userNickName : "Guest" + std::to_string(dummyId);

		GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

		PlayerRef player = Horang::MakeShared<Player>();
		player->uid = dummyId;
		player->id = id;
		player->nickname = nickName;
		player->ownerGameSession = gameSession;

		gameSession->_player = player;

		this->Connect(gameSession);

		// 성공 동작
		Protocol::S_SIGNIN_OK packet;
		packet.set_uid(player->uid);
		packet.set_nickname(player->nickname);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		session->Send(sendBuffer);

		log << "Auto Login Success";
		dummyId++;
		return;
	}

	{
		auto gameSession = static_pointer_cast<GameSession>(session);
		if (gameSession->_player != nullptr)
		{
			Protocol::S_ERROR packet;
			packet.set_errorcode(static_cast<int32>(ErrorCode::SIGNIN_AUTOLOIGN_FAIL));

			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
			session->Send(sendBuffer);

			log << "Auto Login Fail";

			return;
		}
	}

	int32 count = 0;
	for (;; dummyId++)
	{
		count++;
		if (count > 1000)
		{
			log << "Count Over";
			ASSERT_CRASH(false);
		}

		if (dummyId > 1000)
		{
			log << "DummyID Over";
			ASSERT_CRASH(false);
		}

		auto dbConn = GDBConnectionPool->Pop();

		DB::SignIn signIn(*dbConn);

		std::string id = "test" + std::to_string(dummyId);
		std::string password = "test" + std::to_string(dummyId);
		std::string nickname = userNickName.length() > 0 ? userNickName : "test" + std::to_string(dummyId);

		WCHAR wid[40] = L"";
		WCHAR wpassword[80] = L"";
		WCHAR wnickName[16] = L"";

		signIn.In_Id(wid, id);
		signIn.In_Password(wpassword, password);

		int32 uid = 0;
		WCHAR nickName[16] = L"";

		signIn.Out_Uid(uid);
		signIn.Out_NickName(nickName);

		ASSERT_CRASH(signIn.Execute());

		if (signIn.Fetch() == true)
		{
			GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

			if (this->isConnect(uid))
			{
				GDBConnectionPool->Push(dbConn);
				continue;
			}

			PlayerRef player = Horang::MakeShared<Player>();
			player->uid = uid;
			player->id = id;
			std::wstring wstr{ nickName };
			player->nickname = std::string(wstr.begin(), wstr.end());
			player->ownerGameSession = gameSession;

			gameSession->_player = player;

			this->Connect(gameSession);

			// 성공 동작
			Protocol::S_SIGNIN_OK packet;
			packet.set_uid(uid);
			packet.set_nickname(boost::locale::conv::utf_to_utf<char>(std::wstring(nickName)));

			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
			session->Send(sendBuffer);

			GDBConnectionPool->Push(dbConn);

			break;
		}
		else
		{
			GDBConnectionPool->Push(dbConn);
			// 로그인 실패 = 계정이 없다
			// 계정 생성

			auto dbConn = GDBConnectionPool->Pop();

			DB::SignUp signUp(*dbConn);

			signUp.In_Id(wid, id);
			signUp.In_Password(wpassword, password);
			signUp.In_NickName(wnickName, nickname);

			int32 result = 0;
			signUp.Out_Result(result);

			ASSERT_CRASH(signUp.Execute());

			if (signUp.Fetch())
			{
				if (result == 1)
				{
					// 계정 생성 성공
					GDBConnectionPool->Push(dbConn);
					continue;
				}
			}

			GDBConnectionPool->Push(dbConn);
		}
	}

	//std::cout << "Auto Login - " << dummyId << std::endl;
	log << "Auto Login : " << dummyId << "Success";
}

void AuthenticationManager::Logout(Horang::PacketSessionRef session)
{
	LogBuffer log("Logout");

	auto gameSession = static_pointer_cast<GameSession>(session);

	if (gameSession->_player == nullptr)
		return;

	log << "Logout : " << gameSession->_player->nickname;

	this->Disconnect(gameSession->_player->uid);

	gameSession->_player = nullptr;

	Protocol::S_SIGNOUT_OK packet;

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
	session->Send(sendBuffer);

	log << "Success";
}

bool AuthenticationManager::isConnect(int32 uid)
{
	return _activeAccount.find(uid) != _activeAccount.end();
}

void SignInJob::Execute()
{
	GAuthentication->SignIn(_session, _id, _password);
}

void SignUpJob::Execute()
{
	GAuthentication->SignUp(_session, _id, _password, _nickname);
}

void DisconnectJob::Execute()
{
	GAuthentication->Disconnect(_uid);
}

void AutoLoginJob::Execute()
{
	GAuthentication->AutoLogin(_session, _nickname);
}

void SignOutJob::Execute()
{
	GAuthentication->Logout(_session);
}
