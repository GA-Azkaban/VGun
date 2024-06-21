#include "Client.h"
#include "TestScene.h"
#include "LoginSceneView.h"
#include "MainMenu.h"
#include "UnitySceneLoaderTest.h"
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
	// 매니저 초기화
	NetworkManager::Instance();
	LobbyManager::Instance();
	MenuManager::Instance();
	RoundManager::Instance();

	// 씬 초기화
	TestScene test;

	API::LoadSceneByName("Test"); 

	//UnitySceneLoaderTest unity;
	//unity.Start();

	//AnimationLoaderTest loader;
	//loader.Start();
	
	//LoginSceneView login;		//Scene Name : Login
	//login.Initialize();
	// 
	//MainMenuScene menu;			//Scene Name : MainMenu
	//menu.Initalize();	
	//
	//LobbySceneView lobby;
	//lobby.Initialize();

	//LoginSceneView login;		//Scene Name : Login
	//login.Initialize();

	//MainMenuScene menu;			//Scene Name : MainMenu
	//menu.Initalize();

	//API::LoadSceneByName("Lobby");

	//InGameSceneView inGame;			//Scene Name : InGame
	//inGame.Initialize();
	//API::LoadSceneByName("InGame");
	//API::SetRecursiveMouseMode(true);
}
