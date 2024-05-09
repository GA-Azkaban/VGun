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
	Protocol::RoomInfo temp;
	NetworkManager::Instance().RecvRoomEnter(temp);
}

void MenuManager::SetRoom(Protocol::RoomInfo roomInfo)
{
	NetworkManager::Instance().SetRoom(roomInfo);
}

void MenuManager::ShowRoomList()
{
	_roomList->OnEnable();
}

void MenuManager::GetRoomList()
{
	NetworkManager::Instance().SendRoomListRequest();
}

void MenuManager::RoomExit()
{
	Protocol::RoomInfo temp;
	NetworkManager::Instance().RecvRoomLeave(temp);
}

void MenuManager::OtherPlayerEnter()
{
	Protocol::RoomInfo temp;
	NetworkManager::Instance().RecvAnotherPlayerEnter(temp);
}

void MenuManager::OtherPlayerExit()
{
	Protocol::RoomInfo temp;
	NetworkManager::Instance().RecvAnotherPlayerLeave(temp);
}

void MenuManager::SetRoomList(HDData::GameObject* roomListCanvas)
{
	_roomList = roomListCanvas;
}

