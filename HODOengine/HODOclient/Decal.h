#pragma once
#include "../HODOengine/HODO_API.h"
#include "Timer.h"

class Decal : public HDData::Script
{
public:
	Decal();
	static Decal* CreateDecal();

	virtual void Update() override;

	void SetEffectOn(Vector3 targetPos);

private:
	HDData::ImageUI* _decalImage;
	Timer _decalTimer;
	HDData::AnimationCurve _curve;
	Vector3 _targetPos;
	HDData::MeshRenderer* _decalMesh;
};
