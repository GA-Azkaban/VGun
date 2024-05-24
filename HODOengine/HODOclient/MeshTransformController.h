#pragma once
#include "../HODOengine/HODO_API.h"

class MeshTransformController : public HDData::Script
{
public:
	MeshTransformController();

	void Start() override;
	void Update() override;

private:
	//float angleX;
	//float angleY;
	//float angleZ;
};
