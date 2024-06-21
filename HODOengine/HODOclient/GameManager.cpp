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
	delete _myInfo;
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
}

HDData::GameObject* GameManager::GetMyObject()
{
	return _myObj;
}

