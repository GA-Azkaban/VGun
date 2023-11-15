#pragma once
#include "Client.h"

/// <summary>
/// 231026 ������
/// 
/// </summary>

#include "../HODOengine/IHODOengine.h"

class Application
{
public:
	Application();
	~Application();

public:
	void Run();

private:
	IHODOengine* _engine;
	Client* _client;
};

