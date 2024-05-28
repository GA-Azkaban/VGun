#include "Client.h"
#include "TestScene.h"
#include "LoginSceneView.h"
#include "MainMenu.h"
#include "UnitySceneLoaderTest.h"
#include "AnimationLoaderTest.h"
#include "InGameSceneView.h"
#include "LobbySceneView.h"

#include "NetworkManager.h"
#include "LobbyManager.h"
#include "MenuManager.h"
#include "RoundManager.h"

Client::Client()
{

}

Client::~Client()
{

}

void Client::Initialize()
{
	NetworkManager::Instance();
	LobbyManager::Instance();
	MenuManager::Instance();

	/*TestScene test;
	test.Start();

	API::LoadSceneByName("Test"); */

	//UnitySceneLoaderTest unity;
	//unity.Start();

	//AnimationLoaderTest loader;
	//loader.Start();
	
	//LoginSceneView login;		//Scene Name : Login
	//login.Initialize();

	//MainMenuScene menu;			//Scene Name : MainMenu
	//menu.Initalize();	

	InGameSceneView inGame;			//Scene Name : InGame
	inGame.Initialize();

	//LobbySceneView lobby;
	//lobby.Initialize();

	//API::LoadSceneByName("Lobby");
	API::LoadSceneByName("InGame");
}
