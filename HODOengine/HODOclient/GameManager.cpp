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

	meshes[0] = "SKM_BadguyTP_X_default.fbx";
	meshes[1] = "SKM_BusinessManTP_X_default.fbx";
	meshes[2] = "SKM_CowboyTP_X_default.fbx";
	meshes[3] = "SKM_CowgirlTP_X_default.fbx";
	meshes[4] = "SKM_GunManTP_X_default.fbx";
	meshes[5] = "SKM_SheriffTP_X_default.fbx";
	meshes[6] = "SKM_WomanTP_X_default.fbx";
	meshes[7] = "SKM_WorkingGirlTP_X_default.fbx";
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
	_myObj->GetComponent<PlayerInfo>()->GetData(_myInfo);
	_myInfo = _myObj->GetComponent<PlayerInfo>();
}

HDData::GameObject* GameManager::GetMyObject()
{
	return _myObj;
}



