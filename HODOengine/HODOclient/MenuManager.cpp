#include "MenuManager.h"
#include "NetworkManager.h"

MenuManager& MenuManager::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new MenuManager;
	}

	return *_instance;
}

MenuManager* MenuManager::_instance = nullptr;

MenuManager::MenuManager()
{
	API::CreateStaticComponent(this);
}

void MenuManager::Start()
{
	int a = 0;
}

void MenuManager::SetMainMenuCanvas(HDData::GameObject* mainCanvas)
{
	_mainMenuCanvas = mainCanvas;
}

void MenuManager::RoomEneter(Protocol::RoomInfo)
{
	//NetworkManager::Instance().RecvRoomEnter();

}

void MenuManager::SetRoom()
{

}

void MenuManager::RoomExit()
{

}

void MenuManager::OtherPlayerEnter()
{

}

void MenuManager::OtherPlayerExit()
{

}

