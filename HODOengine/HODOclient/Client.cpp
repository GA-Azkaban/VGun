﻿#include "Client.h"
#include "TestScene.h"
#include "LoginSceneView.h"
#include "TrainigSceneView.h"
#include "MainMenu.h"
#include "InGameSceneView.h"
#include "LobbySceneView.h"

#include "NetworkManager.h"
#include "LobbyManager.h"
#include "MenuManager.h"
#include "RoundManager.h"
#include "SoundManager.h"
#include "GameSetting.h"



Client::Client()
{

}

Client::~Client()
{

}

void Client::Initialize()
{	
	// 매니저 초기화
	//GameSetting::Instance();
	//SoundManager::Instance();
	//NetworkManager::Instance();
	//LobbyManager::Instance();
	//MenuManager::Instance();
	//RoundManager::Instance();
	
	// 씬 초기화
	//TestScene test;
	//API::LoadSceneByName("Test");

	//UnitySceneLoaderTest unity;
	//unity.Start();

	//AnimationLoaderTest loader;
	//loader.Start(); 

	//LoginSceneView login;		//Scene N@ame : Login
	//login.Initialize();
	//
	//MainMenuScene menu;			//Scene Name : MainMenu
	//menu.Initalize();
	//
	//LobbySceneView lobby;		//Scene Name : Lobby
	//lobby.Initialize();
	//
	InGameSceneView inGame;			//Scene Name : InGame
	inGame.Initialize();
	//
	//API::LoadSceneByName("Login");

	//InGameSceneView inGame;			//Scene Name : InGame
	//inGame.Initialize();
	API::LoadSceneByName("InGame");
	//API::SetRecursiveMouseMode(true);
}
