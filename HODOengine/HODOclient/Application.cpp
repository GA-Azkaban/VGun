#include "Application.h"

Application::Application()
{
	_engine = CreateEngine();
	_client = new Client;
}

Application::~Application()
{
	delete _client;
	ReleaseEngine(_engine);
}

void Application::Run()
{
	_engine->Initialize();
	_client->Initialize();
	_engine->Loop();
	_engine->Finalize();
}
