#pragma once
#include "../HODOengine/HODO_API.h"
#include "Types.h"

#include "Enum.pb.h"
#include "Struct.pb.h"
#include "Protocol.pb.h"

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

public: // 에러 코드
	void RecvFail(int32 errorCode);

public: // 계정 관련
	// Debug
	void SendAutoLogin();

	void SendLogin(std::string id, std::string password);
	void SendCreateAccount(std::string id, std::string password, std::string nickname);

	void RecvLogin(int32 uid, std::string id);
	void RecvCreateAccount();

public: // 방 리스트 관련
	void SendRoomListRequest();
	void RecvRoomList(Protocol::S_ROOM_LIST roomList);

public: // 방 관련
	void SendRoomEnter(std::string roomCode, std::string password = "");
	void SendRoomLeave();

	void RecvRoomEnter(Protocol::RoomInfo roomInfo);
	void RecvRoomLeave(Protocol::RoomInfo roomInfo);

	void SetRoom();

public:
	void RecvAnotherPlayerEnter(Protocol::RoomInfo roomInfo);
	void RecvAnotherPlayerLeave(Protocol::RoomInfo roomInfo);

public: // 게임 관련
	void SendGameStart();
	void RecvGameStart();

public: // 업데이트
	void SendPlayUpdate(Protocol::PlayerData playerData);
	void RecvPlayUpdate(Protocol::S_PLAY_UPDATE playUpdate);

public: // 동작
	// Todo 총 발사
	// Todo

public:
	bool IsConnected();
	void SetConnect(bool isConnect);

private:
	Horang::ClientServiceRef _service;
	bool _isConnect;
};

