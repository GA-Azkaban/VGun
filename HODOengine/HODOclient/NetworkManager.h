#pragma once
#include "../HODOengine/HODO_API.h"
#include "Types.h"
#include "Enum.pb.h"
#include "Struct.pb.h"

class NetworkManager : public HDData::Script
{
public:
	static NetworkManager& Instance();

private:
	static NetworkManager* _instance;
	NetworkManager();
	~NetworkManager() = default;

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	void SendLogin(std::string id, std::string password);
	void SendCreateAccount(std::string id, std::string password, std::string nickname);
	
public:
	void RecvFail(int32 errorCode);
	void RecvLogin(int32 uid, std::string id);

public:
	bool IsConnected();

private:
	Horang::ClientServiceRef _service;
	bool _isConnect;
};

