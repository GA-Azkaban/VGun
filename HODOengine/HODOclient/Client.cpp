#include "Client.h"
#include "TestScene.h"
#include "LoginSceneView.h"
#include "MainMenu.h"
#include "UnitySceneLoaderTest.h"
#include "AnimationLoaderTest.h"
#include "InGameSceneView.h"
#include "LobbySceneView.h"
#include "TrainingSceneView.h"

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

	//TestScene test;
	//test.Start();

	LoginSceneView login;		//Scene Name : Login
	login.Initialize();

	MainMenuScene menu;			//Scene Name : MainMenu
	menu.Initalize();

	//UnitySceneLoaderTest unity;
	//unity.Start();

	//AnimationLoaderTest loader;
	//loader.Start();

	//InGameSceneView inGame;			//Scene Name : InGame
	//inGame.Initialize();

	LobbySceneView lobby;
	lobby.Initialize();

	TrainingSceneView train;
	train.Initialize();

	API::LoadSceneByName("Login");
}
