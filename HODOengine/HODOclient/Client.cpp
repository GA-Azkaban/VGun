﻿#include "Client.h"
#include "TestScene.h"
#include "LoginSceneView.h"
#include "MainMenu.h"
#include "UnitySceneLoaderTest.h"

Client::Client()
{

}

Client::~Client()
{

}

void Client::Initialize()
{
	//NetworkManager::Instance();

	//TestScene test;
	//test.Start();

	//LoginSceneView login;
	//login.Initialize();

	//MainMenuScene menu;
	//menu.Initalize();

	UnitySceneLoaderTest unity;
	unity.Start();

	//API::LoadSceneByName("MainLobby");
	API::LoadSceneByName("Scene2");
}
