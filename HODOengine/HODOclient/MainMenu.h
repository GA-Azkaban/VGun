#pragma once
#include "../HODOengine/HODO_API.h"

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
