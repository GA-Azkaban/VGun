#pragma once
#include "Struct.pb.h"
#include "../HODOengine/HODO_API.h"

class MenuManager : public HDData::Script
{
public:
	static MenuManager& Instance();

private:
	static MenuManager* _instance;
	MenuManager();
	~MenuManager() = default;

	HDData::GameObject* _this = nullptr;
public:
	void Start() override;

public:
	void SetMainMenuCanvas(HDData::GameObject* mainCanvas);

	void RoomEneter(Protocol::RoomInfo);
	void SetRoom();
	void RoomExit();

	void OtherPlayerEnter();
	void OtherPlayerExit();


public:
	// canvas
	HDData::GameObject* _mainMenuCanvas;

	// btn
	HDData::GameObject* _playBtn;
	HDData::GameObject* _preferencesBtn;	// 환경설정 버튼
	HDData::GameObject* _exitbtn;
};

