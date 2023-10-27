#include "Client.h"

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
}

void Client::Loop()
{
	_engine->Loop();
}

void Client::Finalize()
{
	_engine->Finalize();
}
