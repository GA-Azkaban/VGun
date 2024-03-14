#include "Client.h"
#include "TestScene.h"
#include "LoginSceneView.h"
#include "UnitySceneLoaderTest.h"

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

	UnitySceneLoaderTest unity;
	unity.Start();
}
