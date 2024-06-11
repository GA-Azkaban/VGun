#include "GameManager.h"

GameManager* GameManager::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new GameManager;
	}

	return _instance;
}

GameManager* GameManager::_instance = nullptr;

GameManager::GameManager()
{
	_myInfo = new PlayerInfo;
}

GameManager::~GameManager()
{
	delete _instance;
}

void GameManager::SetMyInfo(PlayerInfo* info)
{
	_myInfo = info;
}

PlayerInfo* GameManager::GetMyInfo()
{
	return _myInfo;
}

void GameManager::SetMyObject(HDData::GameObject* obj)
{
	_myObj = obj;
	PlayerInfo* info = obj->GetComponent<PlayerInfo>();
	SetMyInfo(info);
}

HDData::GameObject* GameManager::GetMyObject()
{
	return _myObj;
}

