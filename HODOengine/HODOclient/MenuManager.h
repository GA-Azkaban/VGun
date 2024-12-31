#pragma once
#include "Struct.pb.h"
#include "GameStruct.h"
#include "../HODOengine/HODO_API.h"

#define DEFAULTWIDTH 1920
#define DEFAULTHEIGHT 1080

struct roomInfoObj
{
	HDData::Button* btn;
	HDData::ImageUI* roomListCanavas;
	HDData::TextUI* id;
	HDData::TextUI* title;
	HDData::ImageUI* userCount;
	HDData::ImageUI* maxCount;
	HDData::ImageUI* slash;
	HDData::ImageUI* currentCount;
	HDData::Button* joinBtn;
	HDData::TextUI* jointxt;
	HDData::ImageUI* isPrivate;
	HDData::ImageUI* isTeam;

	void Clear()
	{
		id->GetGameObject()->SetSelfActive(false);
		title->GetGameObject()->SetSelfActive(false);
		maxCount->GetGameObject()->SetSelfActive(false);
		currentCount->GetGameObject()->SetSelfActive(false);
		userCount->GetGameObject()->SetSelfActive(false);
		slash->GetGameObject()->SetSelfActive(false);
		joinBtn->GetGameObject()->SetSelfActive(false);
		jointxt->GetGameObject()->SetSelfActive(false);
		//isPrivate->GetGameObject()->SetSelfActive(false);
		//isTeam->GetGameObject()->SetSelfActive(false);
	}
};

enum class screenSize
{
	option1600 = 1,	// default	1600x900
	option1920 = 2,	// 1920x1080
	option2550 = 3,	// 2550x1440
	option4 = 4
};

enum class screenMode
{
	FULL_SCREEN = 1,
	BORDERLESS_SCREEN,
	WINDOWS_SCREEN
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
	void Update() override;


private:
	screenSize _prevSize;
	screenSize _size;
	screenMode _mode;

	bool _isModeChange = true;

public:
	//Test
	void SetObjectPosition2560();
	void SetObjectPosition1920();

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
	void SetScreenSize(int optionNum);
	std::string& GetScreenSize();

	void SetScreenMod(int optionNum);
	std::string& GetScreenMod();

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

	// screenSize
	std::string _screenSize = ("1920x1080(60Hz)");	// default

	// screenMod
	std::string _screenMod = ("windowed");
};

