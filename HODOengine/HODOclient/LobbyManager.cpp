#include "LobbyManager.h"

LobbyManager& LobbyManager::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new LobbyManager;
	}

	return *_instance;
}

LobbyManager* LobbyManager::_instance = nullptr;

LobbyManager::LobbyManager()
{

}

void LobbyManager::Start()
{

}

void LobbyManager::Login(std::string id, std::string password)
{

}

void LobbyManager::Join()
{
	_joinCanvas->OnEnable();
}

void LobbyManager::ExitJoin()
{
	_joinCanvas->OnDisable();
}

void LobbyManager::SetLobbyMainCanvas(HDData::GameObject* mainCanvas)
{
	_mainCanvas = mainCanvas;
}

void LobbyManager::SetJoinCanvas(HDData::GameObject* joinCanvas)
{
	_joinCanvas = joinCanvas;
}
