#pragma once
#include "../HODOengine/HODO_API.h"

/// <summary>
/// [신윤제] 게임의 진입 화면.
/// 로그인, 회원가입, 타이틀 화면을 담당한다.
/// </summary>

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
};
