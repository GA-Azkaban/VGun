#pragma once

class AuthenticationManager : public Horang::JobQueue
{
public:
	AuthenticationManager();

public:
	void SignIn();
	void SignUp();

private:
};

extern AuthenticationManager GAuthentication;

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
	SignUpJob(Horang::PacketSessionRef session, std::string id, std::string password,std::string nickname)
		: _session(session), _id(id), _password(password),_nickname(nickname)
	{}

	virtual void Execute() override;

private:
	Horang::PacketSessionRef _session;
	std::string _id;
	std::string _password;
	std::string _nickname;
};