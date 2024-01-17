#pragma once
#include "../HODOengine/HODO_API.h"

class LoginSceneView
{
public:
	LoginSceneView();
	~LoginSceneView();

	void Initialize();

private:
	HDData::Scene* _scene;
};

