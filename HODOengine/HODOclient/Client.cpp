#include "Client.h"
#include "TestScene.h"

Client::Client()
{
	_engine = CreateEngine();
	_engine->Initialize();
}

Client::~Client()
{
	ReleaseEngine(_engine);
}

void Client::Initialize()
{
	_test = new TestScene;
	_test->Start();
}

void Client::Loop()
{
	_engine->Loop();
	_test->Update();
}

void Client::Finalize()
{
	_engine->Finalize();
}
