#pragma once
#include "../HODOengine/HODO_API.h"

class TrainingSceneView
{
public:
	TrainingSceneView();
	~TrainingSceneView();

	void Initialize();

private:
	HDData::Scene* _scene;
};

