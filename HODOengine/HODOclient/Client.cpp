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

	TestScene test;
	test.Start();

	LoginSceneView login;
	login.Initialize();

	MainMenuScene menu;
	menu.Initalize();

	//UnitySceneLoaderTest unity;
	//unity.Start();

	//AnimationLoaderTest loader;
	//loader.Start();

	//InGameSceneView inGame;
	//inGame.Initialize();

	LobbySceneView lobby;
	lobby.Initialize();

	API::LoadSceneByName("Test Scene");
}
