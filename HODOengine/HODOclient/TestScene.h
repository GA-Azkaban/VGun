#pragma once
#include "../HODOengine/HODO_API.h"

class TestScene
{
public:
	TestScene();
	~TestScene();

	void Start();

private:
	hodoData::Scene* _scene;
};

