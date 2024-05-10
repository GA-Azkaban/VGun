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

void MenuManager::RoomEnter()
{

}

void MenuManager::SetRoom()
{
	
}

void MenuManager::ShowRoomListCanvas(bool isShow)
{
	_roomListCanvas->SetSelfActive(isShow);

	if (!isShow) return;

	NetworkManager::Instance().SendRoomListRequest();

	for (auto& data : _roomList)
	{

	}
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

std::vector<roominfo*>& MenuManager::GetRoomList()
{
	return _roomList;
}

void MenuManager::SetRoomListCanvas(HDData::GameObject* obj)
{
	_roomListCanvas = obj;
}

