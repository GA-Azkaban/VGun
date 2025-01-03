﻿#pragma once
#include <vector>
#include "../HODOengine/HODO_API.h"
#include "PlayerInfo.h"
#include "LobbySceneView.h"

enum errorNum
{
	LOGIN_FAIL = 1001,
	ID_DUPLICATION = 1002,
	NICKNAME_DUPLICATION = 1003,
	SIGNUP_FAIL = 1004,
};

struct RoomData
{
	int roomid;
	
	bool isPrivate;

	std::string roomName;
	std::string password;

	int currentPlayerCount;

	std::vector<PlayerInfo*> _players;
};

class LobbyManager : public HDData::Script
{
public:
	static LobbyManager& Instance();

private:
	static LobbyManager* _instance;
	LobbyManager();
	~LobbyManager() = default;

	HDData::GameObject* _this;

public:
	void Start() override;
	void Update() override;

public:
	// 로그인 화면 기능
	void Login(std::string, std::string);
	void Join();
	void ExitJoin();
	void MakeNewAccount(std::string, std::string, std::string);

	void LoginFAIL(int errorCode);

	void LoginSucess(int uid,std::string nickname);

	void showOff(HDData::GameObject*);
	void showOn(HDData::GameObject*);

public:
	void SetLobbyMainCanvas(HDData::GameObject* mainCanvas);
	void SetFadeCanvas(HDData::GameObject* fadeCanvas);
	void SetJoinCanvas(HDData::GameObject* joinCanvas);

	void SetidDupl(HDData::GameObject*);
	void SetnameDule(HDData::GameObject*);
	void SetSucessCanvas(HDData::GameObject* sucessCanvas);
	void SetFailCanvas(HDData::GameObject* failCanvas);

	void SetSignupSuccess(HDData::GameObject* obj);
	void SetSignupFail(HDData::GameObject*);
	void ShowSignSuccess();

	void SetInGameStartButton(HDData::GameObject* button);
	//void SetInGameReadyButton(HDData::GameObject* button);

private:
	// 메인 화면
	HDData::GameObject* _mainCanvas;
	// fade in out
	HDData::GameObject* _fadeCanvas;
	// 회원가입
	HDData::GameObject* _joinCanvas;
	// 가입 성공 || 실패
	HDData::GameObject* _loginSucessCanvas;
	HDData::GameObject* _loginFailCanvas;

	HDData::GameObject* _iddupl;
	HDData::GameObject* _namedupl;

	HDData::GameObject* _signupSuccess;
	HDData::GameObject* _signupFail;

	// 게임 로비
	HDData::GameObject* _startButton;
	HDData::GameObject* _settingButton;
	HDData::GameObject* _quitButton;
	HDData::GameObject* _logoImage;

	// 인게임 로비
	HDData::GameObject* _inGameStartButton;

public:
	RoomData* GetRoomData();
	void RoomEnterFAIL(int errorCode);
	void RoomEnterSUCCESS();
	void RoomLeaveSuccess();

private:
	RoomData* _roomData;

	HDData::GameObject* _roomEnterFailCanvas;
	HDData::GameObject* _roomPasswordFailCanvas;

public:
	// 방 내부의 플레이어 정보를 관리
	void RefreshRoom();

public:
	std::vector<HDData::GameObject*>& GetPlayerObjects();
	std::vector<HDData::GameObject*>& GetNickNameObjects();
	int GetPlayerNum();

private:
	int _playerNum;
	// 씬 내부에 계속 존재하는 오브젝트들
	std::vector<HDData::GameObject*> _playerObjs;
	std::vector<HDData::GameObject*> _nickNameIndex;

public:
	// 플레이어 캐릭터 타입을 관리
	bool isUsed[6] = { false };

	std::pair<bool, int> character[6];
};
