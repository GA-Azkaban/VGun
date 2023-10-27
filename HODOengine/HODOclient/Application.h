#pragma once
#include "Client.h"

/// <summary>
/// 231026 ¿À¼ö¾È
/// 
/// </summary>

class Application
{
public:
	Application();
	~Application();

public:
	void Run();

private:
	Client* _client;
};

