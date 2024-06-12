﻿#pragma once
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

	void SendRoomCreate(std::string roomName, std::string password = "", int32 maxPlayerCount = 6, bool isPrivate = false, bool isTeam = true);
	void SetRoom(Protocol::RoomInfo roomInfo);

public: // 다른 플레이어
	void RecvAnotherPlayerEnter(Protocol::RoomInfo roomInfo);
	void RecvAnotherPlayerLeave(Protocol::RoomInfo roomInfo);

public: // 강퇴
	void SendKickPlayer(std::string targetNickName);
	void RecvKickPlayer(Protocol::RoomInfo roomInfo);

public: // 팀 색상 변경
	void SendChangeTeamColor(Protocol::eTeamColor teamColor, std::string targetNickName = "");
	void RecvChangeTeamColor(Protocol::RoomInfo roomInfo);

public: // 게임 관련
	void SendGameStart();
	void RecvGameStart();

public: // 트랜스폼 동기화
	void SendPlayerTranform();
	void RecvPlayerTranform();


public: // 업데이트
	void SendPlayUpdate();
	void RecvPlayUpdate(Protocol::S_PLAY_UPDATE playUpdate);

public: // 동작
	// Todo 총 발사
	// Todo

public:
	void SetConnect(bool isConnect);
	void Connected();
	void Disconnected();
	bool IsConnected();

private:
	Horang::ClientServiceRef _service;
	bool _isConnect;


	// 보간 (을 서버에서 하는게 맞을까나)
public:
	void Interpolation(HDData::Transform* current, Vector3 serverPos, Quaternion serverRot, float intermediateValue);

private:
	// 
	HDData::Transform* currentTransform[5];
	Vector3 serverPosition[5];
	Quaternion serverRotation[5];
};

