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
	void SetMyInfo(PlayerInfo* info);
	PlayerInfo* GetMyInfo();

private:
	PlayerInfo* _myInfo;
};

