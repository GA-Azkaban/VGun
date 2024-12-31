#pragma once
#include "../HODOengine/HODO_API.h"

/// <summary>
/// [신윤제] 로그인을 마쳤을 때 나오는 화면.
/// 게임 시작, 설정, 종료, 방 목록, 방 생성 등의 기능을 담당한다.
/// </summary>

class MenuManager;

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
