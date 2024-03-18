#pragma once

class AuthenticationManager
{
public:
	AuthenticationManager();

public:
	void SignIn();
	void SignUp();

public:
	void PushJob(JobRef job);
	void FlushJob();

private:
	USE_LOCK;

	JobQueue _jobs;
};

extern AuthenticationManager GAuthentication;

class SignInJob : public IJob
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

class SignUpJob : public IJob
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