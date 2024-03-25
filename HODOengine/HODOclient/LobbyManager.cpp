#include "Types.h"
#include "LobbyManager.h"
#include "NetworkManager.h"


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
	API::CreateStaticComponent(this);
}

void LobbyManager::Start()
{
	int a = 0;
}

void LobbyManager::Login(std::string id, std::string password)
{
	std::string testid = id;
	std::string testpw = password;
}

void LobbyManager::Join()
{
	_joinCanvas->OnEnable();
}

void LobbyManager::ExitJoin()
{
	_joinCanvas->OnDisable();
}

void LobbyManager::MakeNewAccount(std::string id, std::string password, std::string nickname)
{
	NetworkManager::Instance().SendCreateAccount(id, password,nickname);
}

void LobbyManager::LoginFAIL(int errorCode)
{
	switch (errorCode)
	{
		case LOGIN_FAIL:

			break;
		case ID_DUPLICATION:
			break;

		case NICKNAME_DUPLICATION:
			break;

		case SIGNUP_FAIL:
			break;

		default:
			std::cout << "Unknown Error Code" << std::endl;
			break;
	}
}

void LobbyManager::SetLobbyMainCanvas(HDData::GameObject* mainCanvas)
{
	_mainCanvas = mainCanvas;
}

void LobbyManager::SetJoinCanvas(HDData::GameObject* joinCanvas)
{
	_joinCanvas = joinCanvas;
}

