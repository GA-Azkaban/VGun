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
	void SetRoom(Protocol::RoomInfo);
	void ShowRoomList();
	void RoomExit();

	void OtherPlayerEnter();
	void OtherPlayerExit();

	void SetRoomList(HDData::GameObject* roomListCanvas);
public:
	// canvas
	HDData::GameObject* _mainMenuCanvas = nullptr;

	// btn
	HDData::GameObject* _playBtn = nullptr;
	HDData::GameObject* _preferencesBtn = nullptr;	// 환경설정 버튼
	HDData::GameObject* _exitbtn = nullptr;

	// RoomList
	HDData::GameObject* _roomList = nullptr;
};

