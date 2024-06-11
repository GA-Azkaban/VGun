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
	// 전체화면으로 자동 시작

	auto height = API::GetScreenHeight();
	auto width = API::GetScreenWidth();

	if (width == 1920)
	{
		_size = screenSize::option1920;
	}
	else if (width == 2560)
	{
		_size = screenSize::option2550;
	}

	_mode = screenMode::FULL_SCREEN;
}

void MenuManager::Update()
{
	if (!_isModeChange) return;

	if (_size == screenSize::option1920)
	{
		SetObjectPosition1920();
		_isModeChange = false;
	}
	if (_size == screenSize::option2550)
	{
		//SetObjectPosition2560();
		_isModeChange = false;
	}
}

void MenuManager::SetObjectPosition1920()
{

}

void MenuManager::SetObjectPosition2560()
{
	//auto uiList = API::GetAllUIList();

	//for (auto& ui : uiList)
	//{
	//	HDData::GameObject* obj = ui->GetGameObject();

	//	// x
	//	int x = obj->GetTransform()->GetPosition().x;
	//	int newX = x / 3 * 4;

	//	// y
	//	int y = obj->GetTransform()->GetPosition().y;
	//	int newY = y / 3 * 4;

	//	obj->GetTransform()->SetPosition(newX, newY, 0);
	//}

	auto uiList = API::GetAllUIList();

	// 원래 해상도와 새로운 해상도의 크기
	float originalWidth = 1920.0f;
	float originalHeight = 1080.0f;
	float newWidth = 2560.0f;
	float newHeight = 1440.0f;

	// 비율 계산
	float widthRatio = newWidth / originalWidth;
	float heightRatio = newHeight / originalHeight;

	for (auto& ui : uiList)
	{
		HDData::GameObject* obj = ui->GetGameObject();

		// 현재 좌표 가져오기
		float currentX = obj->GetTransform()->GetPosition().x;
		float currentY = obj->GetTransform()->GetPosition().y;

		// 새로운 좌표 계산
		float newX = currentX * widthRatio;
		float newY = currentY * heightRatio;

		// 새 좌표 설정
		obj->GetTransform()->SetPosition(newX, newY, 0);
	}
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

void MenuManager::ShowCheckEnterCanvas(bool isShow)
{
	_checkEnterCanvas->SetSelfActive(isShow);
}

void MenuManager::ShowCheckPasswordCanvas(bool isShow)
{
	_checkPasswordCanvas->SetSelfActive(isShow);
}

void MenuManager::SetCheckEnterCanvas(HDData::GameObject* canvas)
{
	_checkEnterCanvas = canvas;
}

void MenuManager::SetInputRoomPasswordCanvas(HDData::GameObject* canvas)
{
	_checkPasswordCanvas = canvas;
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
		case 0: { return "flair_number_0_outline.png"; } break;
		case 1: { return "flair_number_1_outline.png"; } break;
		case 2: { return "flair_number_2_outline.png"; } break;
		case 3: { return "flair_number_3_outline.png"; } break;
		case 4: { return "flair_number_4_outline.png"; } break;
		case 5: { return "flair_number_5_outline.png"; } break;
		case 6: { return "flair_number_6_outline.png"; } break;
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

void MenuManager::SetScreenSize(int OptionNum)
{
	switch (static_cast<screenSize>(OptionNum))
	{
		case screenSize::option1600:
			_screenSize = ("1600x900(60Hz)");
			API::Resize(1600, 900);
			break;
		case screenSize::option1920:
			_screenSize = ("1920x1080(60Hz)");
			API::Resize(1920, 1080);
			break;
		case screenSize::option2550:
			_screenSize = ("2550x1440(60Hz)");
			API::Resize(2550, 1440);
			break;
		default:
			break;
	}
}

std::string& MenuManager::GetScreenSize()
{
	return _screenSize;
}

void MenuManager::SetScreenMod(int OptionNum)
{
	// 기능 추가할 예정
	switch (static_cast<screenMode>(OptionNum))
	{
		case screenMode::FULL_SCREEN:
			_screenMod = ("FULLSCREEN");
			break;
		case screenMode::BORDERLESS_SCREEN:
			_screenMod = ("BORDERLESS");
			break;
		case screenMode::WINDOWS_SCREEN:
			_screenMod = (" WINDOWED ");
			break;
		default:
			break;
	}
}

std::string& MenuManager::GetScreenMod()
{
	return _screenMod;
}

void MenuManager::RenderRoomList()
{
	if (_roomList.size() == 0) return;

	pageCount = _roomList.size() / 5;
	currentPage = 1;

	int roomCount = 5;

	if (_roomList.size() % 5 > 0) pageCount++;

	if (_roomList.size() < 5) roomCount = _roomList.size();

	for (int i = 1 * currentPage; i < (roomCount + 1) * currentPage; ++i)
	{
		_roomObject[i - 1].id->SetText(std::to_string(_roomList[i - 1]->id));
		_roomObject[i - 1].title->SetText(_roomList[i - 1]->title);

		if (_roomList[i - 1]->isTeam)
		{
			_roomObject[i - 1].isTeam->SetImage("team.png");
		}
		else
		{
			_roomObject[i - 1].isTeam->SetImage("no_team.png");
		}

		if (_roomList[i - 1]->isPrivate)
		{
			_roomObject[i - 1].isPrivate->SetImage("private.png");
		}
		else
		{
			_roomObject[i - 1].isPrivate->SetImage("no_private.png");
		}

		_roomObject[i - 1].maxCount->SetImage(GetNumberImage(_roomList[i - 1]->maxPlayerCount));
		_roomObject[i - 1].currentCount->SetImage(GetNumberImage(_roomList[i - 1]->currentPlayerCount));

		_roomObject[i - 1].id->GetGameObject()->SetSelfActive(true);
		_roomObject[i - 1].title->GetGameObject()->SetSelfActive(true);
		_roomObject[i - 1].isTeam->GetGameObject()->SetSelfActive(true);
		_roomObject[i - 1].isPrivate->GetGameObject()->SetSelfActive(true);
		_roomObject[i - 1].maxCount->GetGameObject()->SetSelfActive(true);
		_roomObject[i - 1].currentCount->GetGameObject()->SetSelfActive(true);
	}
}

