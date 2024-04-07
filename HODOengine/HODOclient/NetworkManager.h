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
	void RecvFail(int32 errorCode);

public:
	void SendLogin(std::string id, std::string password);
	void SendCreateAccount(std::string id, std::string password, std::string nickname);
	
	void RecvLogin(int32 uid, std::string id);
	void RecvCreateAccount();

public:
	void SendRoomEnter(std::string roomCode);
	void SendRoomLeave();

	void RecvRoomEnter(Protocol::RoomInfo roomInfo);
	void RecvRoomLeave(Protocol::RoomInfo roomInfo);

	void SetRoom();

public:
	void RecvAnotherPlayerEnter(Protocol::RoomInfo roomInfo);
	void RecvAnotherPlayerLeave(Protocol::RoomInfo roomInfo);

public:
	bool IsConnected();
	void SetConnect(bool isConnect);

private:
	Horang::ClientServiceRef _service;
	bool _isConnect;
};

