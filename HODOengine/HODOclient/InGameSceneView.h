#pragma once
#include "../HODOengine/HODO_API.h"

/// <summary>
/// [오수안] 게임 시작 버튼을 눌렀을 때 나오는 씬.
/// 게임의 한 라운드를 담당한다.
/// </summary>

class InGameSceneView
{

public:
	InGameSceneView();
	~InGameSceneView();

	void Initialize();

private:
	HDData::Scene* _scene;

	int _bulletCount = 0;
	std::string _tempBullet;

	std::string& convertBullet(int bullet);
};

