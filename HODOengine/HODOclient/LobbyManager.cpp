#include "LobbyManager.h"

LobbyManager* LobbyManager::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new LobbyManager;
	}

	return _instance;
}

LobbyManager* LobbyManager::_instance = nullptr;

