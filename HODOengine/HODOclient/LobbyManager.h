#pragma once
#include "../HODOengine/HODO_API.h"

class LobbyManager : public HDData::Script
{
public:
	static LobbyManager* Instance();

private:
	static LobbyManager* _instance;
	LobbyManager();
	~LobbyManager() = default;

public:
	// 로그인 화면 기능
	void Login();
	void Join();

public:
	void SetLobbyMainCanvas(HDData::GameObject* mainCanvas);
	void SetIDTextBox();


private:
	// 메인 화면
	HDData::GameObject* _mainCanvas;	

	// 로그인
	HDData::GameObject* _idTextBox;
	HDData::GameObject* _passwordTextBox;
	HDData::GameObject* _nicknameTextBox;

	// 게임 로비
	HDData::GameObject* _startButton;
	HDData::GameObject* _settingButton;
	HDData::GameObject* _quitButton;
	HDData::GameObject* _logoImage;

	// 게임 설정
	HDData::GameObject* _gameSettingCanvas;

	HDData::GameObject* _quitSettingButton;
	HDData::GameObject* _saveSettingButton;
	HDData::GameObject* _clearSettingButton;

	HDData::GameObject* _monitorSettingButton;
	HDData::GameObject* _monitorAspectRatio;
	
	HDData::GameObject* _inputSettingButton;
	HDData::GameObject* _walkFrontSetBox;

	HDData::GameObject* _soundSettingButton;
	HDData::GameObject* _musicVolumeSlider;
	HDData::GameObject* _fxVolumeSlider;

	HDData::GameObject* _joinCanvas;	// 참가 화면

};

