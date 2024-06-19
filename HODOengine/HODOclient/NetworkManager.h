#pragma once
#include "../HODOengine/HODO_API.h"
#include "Types.h"

#include "Enum.pb.h"
#include "Struct.pb.h"
#include "Protocol.pb.h"
#include "PlayerInfo.h"

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
	// 방 시작
	void RecvRoomStart(Protocol::RoomInfo roomInfo, Protocol::GameRule gameRule);

	// N초 뒤 게임 시작시 호출
	void RecvGameStart();

public: // 트랜스폼 동기화
	void SendPlayerTranform();
	void RecvPlayerTranform();


public: // 업데이트
	void SendPlayUpdate();
	void RecvPlayUpdate(Protocol::S_PLAY_UPDATE playUpdate);

public: // 인게임
	void SendPlayJump(PlayerInfo* playerinfo);
	void RecvPlayJump(Protocol::PlayerData playerData);

	void SendPlayShoot(HDData::Transform* transform, uint64 hitTargetUid = 0, Protocol::eHitLocation hitLocation = Protocol::eHitLocation::HIT_LOCATION_NO_HIT);
	void RecvPlayShoot(Protocol::PlayerData playerData);
	void RecvPlayShoot(Protocol::PlayerData playerData, Protocol::PlayerData hitPlayerData, Protocol::eHitLocation hitLocation);

	void RecvPlayKillDeath(Protocol::PlayerData deathPlayerData, Protocol::PlayerData killPlayerData);

	// 리스폰 지점 추가해야함
	void RecvPlayRespawn(Protocol::PlayerData playerData);

	// 구르기
	void SendPlayRoll(Protocol::PlayerData playerData);
	void RecvPlayRoll(Protocol::PlayerData playerData);

	// 재장전
	void SendPlayReload(Protocol::PlayerData playerData);
	void RecvPlayReload(Protocol::PlayerData playerData);

public:
	void Connected();
	void Disconnected();
	bool IsConnected();


private:
	Horang::ClientServiceRef _service;
	bool _isConnect;

private:
	Protocol::PlayerData data;
	Protocol::PlayerData* ConvertPlayerInfoToData(PlayerInfo* info);

	// 보간
public:
	void Interpolation(HDData::Transform* current, Vector3 serverPos, Quaternion serverRot, float intermediateValue);

private:
	Vector3 serverPosition[5];
	Quaternion serverRotation[5];

	// 애니메이션
public:
	Protocol::eAnimationState ConvertStateToEnum(const std::string& state);
	ePlayerState ConvertAnimationStateToEnum(Protocol::eAnimationState state);
	//std::string ConvertAnimationStateToString(Protocol::eAnimationState state);

};

