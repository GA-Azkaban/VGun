#pragma once
#include "PlayerInfo.h"
#include "PlayerState.h"

class GameManager
{
public:
	static GameManager* Instance();

private:
	static GameManager* _instance;
	GameManager();
	~GameManager();

public:
	void Logout();
	void QuitGame();

public:
	void SetMyInfo(PlayerInfo* info);
	PlayerInfo* GetMyInfo();
	void SetMyObject(HDData::GameObject* obj);
	HDData::GameObject* GetMyObject();

private:
	HDData::GameObject* _myObj;
	PlayerInfo* _myInfo;

public:
	// 캐릭터 메시 관리용
	std::string meshes[8];
};

