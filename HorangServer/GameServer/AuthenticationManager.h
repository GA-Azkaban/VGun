#pragma once

class AuthenticationManager : public Horang::JobQueue
{
public:
	AuthenticationManager();

public:
	bool SignIn(Horang::PacketSessionRef session, std::string id, std::string password);
	bool SignUp(Horang::PacketSessionRef session, std::string id, std::string password, std::string nickname);

	bool Disconnect(int32 uid);

public: // Debg
	void PrintActiveAccount();
	void AutoLogin(Horang::PacketSessionRef session, std::string userNickName);

	void Logout(Horang::PacketSessionRef session);

private:
	bool Connect(GameSessionRef session);
	bool isConnect(int32 uid);

private:
	Horang::HashMap<int32, GameSessionRef> _activeAccount;
};

using AuthenticationManagerRef = std::shared_ptr<AuthenticationManager>;
extern AuthenticationManagerRef GAuthentication;


class SignInJob : public Horang::IJob
{
public:
	SignInJob(Horang::PacketSessionRef session, std::string id, std::string password)
		: _session(session), _id(id), _password(password)
	{}

	virtual void Execute() override;

private:
	Horang::PacketSessionRef _session;
	std::string _id;
	std::string _password;
};

class SignUpJob : public Horang::IJob
{
public:
	SignUpJob(Horang::PacketSessionRef session, std::string id, std::string password, std::string nickname)
		: _session(session), _id(id), _password(password), _nickname(nickname)
	{}

	virtual void Execute() override;

private:
	Horang::PacketSessionRef _session;
	std::string _id;
	std::string _password;
	std::string _nickname;
};

class DisconnectJob : public Horang::IJob
{
public:
	DisconnectJob(int32 uid)
		: _uid(uid)
	{}

	virtual void Execute() override;

private:
	int32 _uid;
};

class AutoLoginJob : public Horang::IJob
{
public:
	AutoLoginJob(Horang::PacketSessionRef session, std::string nickName)
		: _session(session), _nickname(nickName)
	{}

	virtual void Execute() override;

private:
	Horang::PacketSessionRef _session;
	std::string _nickname;
};

class SignOutJob : public Horang::IJob
{
public:
	SignOutJob(Horang::PacketSessionRef session)
		: _session(session)
	{}

	virtual void Execute() override;

private:
	Horang::PacketSessionRef _session;
};