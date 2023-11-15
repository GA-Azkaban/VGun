#include "Client.h"
#include "TestScene.h"

Client::Client()
{
	_engine = CreateEngine();
}

Client::~Client()
{
	ReleaseEngine(_engine);
}

void Client::Initialize()
{
	_engine->Initialize();
}

void Client::Loop()
{
	_engine->Loop();
}

void Client::Finalize()
{
	_engine->Finalize();
}
