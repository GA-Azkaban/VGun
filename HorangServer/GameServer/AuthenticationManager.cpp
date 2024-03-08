#include "pch.h"
#include "AuthenticationManager.h"

#include "DBConnectionPool.h"
#include "DBConnector.h"

#include "ClientPacketHandler.h"
#include "Player.h"
#include "GameSession.h"

AuthenticationManager GAuthentication;

AuthenticationManager::AuthenticationManager()
{

}

void AuthenticationManager::SignIn()
{

}

void AuthenticationManager::SignUp()
{

}

void AuthenticationManager::PushJob(JobRef job)
{
	WRITE_LOCK;

	_jobs.Push(job);
}

void AuthenticationManager::FlushJob()
{
	WRITE_LOCK;

	while (true)
	{
		JobRef job = _jobs.Pop();
		if (job == nullptr)
			break;

 		job->Execute();
	}
}

void SignInJob::Execute()
{
	if (_id.length() > 40 || _password.length() > 80)
		return;

	auto dbConn = GDBConnectionPool->Pop();

	DB::SignIn signIn(*dbConn);

	WCHAR id[40] = L"";
	WCHAR password[80] = L"";

	signIn.In_Id(id, _id);
	signIn.In_Password(password, _password);

	int32 uid = 0;
	WCHAR nickName[16] = L"";

	signIn.Out_Uid(uid);
	signIn.Out_NickName(nickName);

	ASSERT_CRASH(signIn.Execute());

	if (signIn.Fetch() == true)
	{
		std::wcout << uid << " : " << nickName << std::endl;

		GameSessionRef gameSession = static_pointer_cast<GameSession>(_session);

		{
			PlayerRef player = Horang::MakeShared<Player>();
			player->uid = uid;
			player->id = _id;
			std::wstring wstr{ nickName };
			player->nickname = std::string(wstr.begin(), wstr.end());
			player->ownerGameSession = gameSession;

			gameSession->_player = player;
		}

		// 성공 동작
		Protocol::S_SIGNIN_OK packet;
		packet.set_uid(uid);
		packet.set_nickname(boost::locale::conv::utf_to_utf<char>(std::wstring(nickName)));

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		_session->Send(sendBuffer);
	}
	else
	{
		// 실패 동작
		Protocol::S_ERROR packet;
		packet.set_errorcode(ErrorCode::SIGNIN_FAIL);

		// Todo Log

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		_session->Send(sendBuffer);
	}

	GDBConnectionPool->Push(dbConn);
}

void SignUpJob::Execute()
{
	if (_id.length() > 40 ||
		_password.length() > 80 ||
		_nickname.length() > 16)
		return;

	auto dbConn = GDBConnectionPool->Pop();

	DB::SignUp signUp(*dbConn);

	WCHAR id[40] = L"";
	WCHAR password[80] = L"";
	WCHAR nickName[16] = L"";

	signUp.In_Id(id, _id);
	signUp.In_Password(password, _password);
	signUp.In_NickName(nickName, _nickname);

	int32 result = 0;
	signUp.Out_Result(result);

	ASSERT_CRASH(signUp.Execute());

	if (signUp.Fetch())
	{
		if (result == 1)
		{
			Protocol::S_SIGNUP_OK packet;

			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
			_session->Send(sendBuffer);
		}
		else
		{
			Protocol::S_ERROR packet;
			// Todo Log

			packet.set_errorcode(result);

			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
			_session->Send(sendBuffer);
		}
	}
	else
	{
		Protocol::S_ERROR packet;

		packet.set_errorcode(ErrorCode::SIGNUP_FAIL);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
		_session->Send(sendBuffer);
	}

	GDBConnectionPool->Push(dbConn);
}
