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

}

void MenuManager::SetRoom(Protocol::RoomInfo)
{
	
}

void MenuManager::ShowRoomList()
{

}

void MenuManager::GetRoomListFromServer()
{
	NetworkManager::Instance().SendRoomListRequest();

}

void MenuManager::ShowRoomListCanvas(bool isShow)
{
	_roomListCanvas->SetSelfActive(isShow);

	if (!isShow) return;

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

std::vector<roominfo*>& MenuManager::GetRoomList()
{
	return _roomList;
}

void MenuManager::SetRoomListCanvas(HDData::GameObject* obj)
{
	_roomListCanvas = obj;
}

std::string MenuManager::GetNumberImage(int num)
{
	switch (num)
	{
		case 1: {return "flair_number_1_outline.png";} break;
		case 2: {return "flair_number_2_outline.png";} break;
		case 3: {return "flair_number_3_outline.png";} break;
		case 4: {return "flair_number_4_outline.png";} break;
		case 5: {return "flair_number_5_outline.png";} break;
		case 6: {return "flair_number_6_outline.png";} break;
		default:
			break;
	}
}

std::string MenuManager::GetIsPrivateImage(bool isPrivate)
{
	return "icon_user_filled.png";
}

std::string MenuManager::GetIsTeamImage(bool isTeam)
{
	return "icon_user_filled.png";
}

