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

void GameManager::Logout()
{
	_myObj = nullptr;
	_myInfo = nullptr;
}

void GameManager::QuitGame()
{
	// TODO) 게임 종료 전에 해야 하는 것들
	API::QuitWindow();
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



