#pragma once
#include "../HODOengine/HODO_API.h"

enum errorNum
{
	LOGIN_FAIL = 1001,
	ID_DUPLICATION = 1002,
	NICKNAME_DUPLICATION = 1003,
	SIGNUP_FAIL = 1004,
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

public:
	// 로그인 화면 기능
	void Login(std::string, std::string);
	void Join();
	void ExitJoin();
	void MakeNewAccount(std::string, std::string, std::string);

	void LoginFAIL(int errorCode);

public:
	void SetLobbyMainCanvas(HDData::GameObject* mainCanvas);
	void SetJoinCanvas(HDData::GameObject* joinCanvas);

private:
	// 메인 화면
	HDData::GameObject* _mainCanvas;
	// 회원가입
	HDData::GameObject* _joinCanvas;

	// 게임 로비
	HDData::GameObject* _startButton;
	HDData::GameObject* _settingButton;
	HDData::GameObject* _quitButton;
	HDData::GameObject* _logoImage;
};
