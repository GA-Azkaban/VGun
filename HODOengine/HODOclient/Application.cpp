#include "Application.h"

Application::Application()
{
	_client = new Client;
}

Application::~Application()
{
	delete _client;
}

void Application::Run()
{
	_client->Initialize();
	_client->Loop();
	_client->Finalize();
}
