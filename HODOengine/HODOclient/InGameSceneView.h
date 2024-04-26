#pragma once
#include "../HODOengine/HODO_API.h"

class InGameSceneView
{

public:
	InGameSceneView();
	~InGameSceneView();

	void Initialize();

private:
	HDData::Scene* _scene;
};

