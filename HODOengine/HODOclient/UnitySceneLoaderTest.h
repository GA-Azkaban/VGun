#pragma once
#include "../HODOengine/HODO_API.h"

class UnitySceneLoaderTest
{
public:
	UnitySceneLoaderTest();
	~UnitySceneLoaderTest();

	void Start();

private:
	HDData::Scene* _scene;
};

