#pragma once
#include "Struct.pb.h"
#include "GameStruct.h"
#include "../HODOengine/HODO_API.h"

struct roomInfoObj
{
	HDData::Button* btn;
	HDData::TextUI* id;
	HDData::TextUI* title;
	HDData::ImageUI* maxCount;
	HDData::ImageUI* currentCount;
	HDData::ImageUI* isPrivate;
	HDData::ImageUI* isTeam;
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

	void RoomEneter(Protocol::RoomInfo);
	void SetRoom(Protocol::RoomInfo);
	void ShowRoomList();
	void GetRoomListFromServer();
	void RoomExit();

	void OtherPlayerEnter();
	void OtherPlayerExit();

	std::vector<roominfo*>& GetRoomList();
	void SetRoomListCanvas(HDData::GameObject* obj);

	void ShowRoomListCanvas(bool isShow);
	void ShowCheckEnterCanvas(bool isShow);
	void ShowCheckPasswordCanvas(bool isShow);

	void SetCheckEnterCanvas(HDData::GameObject* canvas);
	void SetInputRoomPasswordCanvas(HDData::GameObject* canvas);

public:
	std::string GetNumberImage(int num);
	std::string GetIsPrivateImage(bool isPrivate);
	std::string GetIsTeamImage(bool isTeam);

public:
	// canvas
	HDData::GameObject* _mainMenuCanvas = nullptr;
	HDData::GameObject* _checkEnterCanvas = nullptr;
	HDData::GameObject* _checkPasswordCanvas = nullptr;

	// btn
	HDData::GameObject* _playBtn = nullptr;
	HDData::GameObject* _preferencesBtn = nullptr;	// 환경설정 버튼
	HDData::GameObject* _exitbtn = nullptr;

	

public:
	void RenderRoomList();

public:
	// room data
	std::vector<roominfo*> _roomList;

	// room obj
	int pageCount = 1;
	int currentPage = 1;
	roominfo* selectedRoomInfo = nullptr;
	HDData::GameObject* _roomListCanvas = nullptr;
	roomInfoObj _roomObject[5];
};

