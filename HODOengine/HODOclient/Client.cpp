#include "Client.h"
#include "TestScene.h"
#include "LoginSceneView.h"
#include "MainMenu.h"
#include "UnitySceneLoaderTest.h"
#include "A.h"
#include "B.h"

Client::Client()
{

}

Client::~Client()
{

}

void Client::Initialize()
{
	TestScene test;
	test.Start();

	//LoginSceneView login;
	//login.Initialize();

	//MainMenuScene menu;
	//menu.Initalize();

	//UnitySceneLoaderTest unity;
	//unity.Start();

	//A a;
	//a.Initialize();

	B b;
	b.Initialize();

	API::LoadSceneByName("Test Scene");
}
