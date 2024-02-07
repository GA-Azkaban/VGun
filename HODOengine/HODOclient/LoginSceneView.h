#pragma once
#include "../HODOengine/HODO_API.h"

class LobbyManager;

class LoginSceneView
{
public:
	LoginSceneView();
	~LoginSceneView();

	void Initialize();

private:
	void LoginView();
	void MainMenu();
	void Lobby();

private:
	HDData::Scene* _scene;

	LobbyManager& _lobbyManager;
};
