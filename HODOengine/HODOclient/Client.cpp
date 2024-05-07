#include "Client.h"
#include "TestScene.h"
#include "LoginSceneView.h"
#include "MainMenu.h"
#include "UnitySceneLoaderTest.h"
#include "AnimationLoaderTest.h"
#include "InGameSceneView.h"
#include "NetworkManager.h"

Client::Client()
{

}

Client::~Client()
{

}

void Client::Initialize()
{
	NetworkManager::Instance();

	//TestScene test;
	//test.Start();

	//LoginSceneView login;
	//login.Initialize();

	MainMenuScene menu;
	menu.Initalize();

	//UnitySceneLoaderTest unity;
	//unity.Start();

	//AnimationLoaderTest loader;
	//loader.Start();

	//API::LoadSceneByName("MainLobby");
	//API::LoadSceneByName("ALT");

	//InGameSceneView inGame;
	//inGame.Initialize();

	//API::LoadSceneByName("InGameScene");
}
