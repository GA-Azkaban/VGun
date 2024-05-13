#pragma once
#include "Struct.pb.h"
#include "GameStruct.h"
#include "../HODOengine/HODO_API.h"

struct roomInfoObj
{
	HDData::GameObject* id;
	HDData::GameObject* title;
	HDData::GameObject* maxCount;
	HDData::GameObject* currentCount;
	HDData::GameObject* isPrivate;
	HDData::GameObject* isTeam;
};


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

	void RoomEnter();
	void SetRoom();
	void ShowRoomListCanvas(bool isShow);
	void RoomExit();

	void OtherPlayerEnter();
	void OtherPlayerExit();

	std::vector<roominfo*>& GetRoomList();
	void SetRoomListCanvas(HDData::GameObject* obj);

public:
	std::string GetNumberImage(int num);
	std::string GetIsPrivateImage(bool isPrivate);
	std::string GetIsTeamImage(bool isTeam);

public:
	// canvas
	HDData::GameObject* _mainMenuCanvas = nullptr;

	// btn
	HDData::GameObject* _playBtn = nullptr;
	HDData::GameObject* _preferencesBtn = nullptr;	// 환경설정 버튼
	HDData::GameObject* _exitbtn = nullptr;

	// RoomList
	HDData::GameObject* _roomListCanvas = nullptr;

public:

public:
	roomInfoObj rooms[4];
	std::vector<roominfo*> _roomList;
};

