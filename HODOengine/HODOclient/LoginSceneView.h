#pragma once
#include "../HODOengine/HODO_API.h"

class LobbyManager;
class FadeInOut;

class LoginSceneView
{
public:
	LoginSceneView();
	~LoginSceneView();

	void Initialize();

private:
	void LoginView();

private:
	HDData::Scene* _scene;

	LobbyManager& _lobbyManager;
	FadeInOut& _fadeInOut;
};
