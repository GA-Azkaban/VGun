﻿#pragma once
#include "../HODOengine/HODO_API.h"

/// <summary>
/// [오수안] 목록에서 방 선택할 시 나오는 화면
/// 플레이어 진입, 팀 변경, 게임 설정, 게임 시작 등의 기능
/// </summary>

class LobbySceneView
{
public:
	LobbySceneView();
	~LobbySceneView();

	void Initialize();

private:
	HDData::Scene* _scene;
};

