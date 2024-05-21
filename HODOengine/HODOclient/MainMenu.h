#pragma once
#include "../HODOengine/HODO_API.h"

/// <summary>
/// [신윤제] 로그인을 마쳤을 때 나오는 화면.
/// 게임 시작, 설정, 종료, 방 목록, 방 생성 등의 기능을 담당한다.
/// </summary>

class MenuManager;

//enum screenSizetemp
//{
//	option1600 = 1,	// default	1600x900
//	option1920 = 2,	// 1920x1080
//	option2550 = 3,	// 2550x1440
//	option4 = 4,
//};

class MainMenuScene
{
public:
	MainMenuScene();
	~MainMenuScene();

	void Initalize();

private:
	void MainMenu();

private:
	HDData::Scene* _scene = nullptr;

	MenuManager& _menuManager;

};
