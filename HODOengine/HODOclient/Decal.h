#pragma once
#include "../HODOengine/HODO_API.h"
#include "Timer.h"
#include <unordered_set>

class Decal : public HDData::Script
{
public:
	Decal();
	~Decal();
	static void CreateDecal();
	static void RemoveAllDecals();

protected:
	virtual void OnEnable() override;
	virtual void Update() override;

private:
	Timer _decalTimer;
	HDData::DecalRenderer* _decalRenderer;

	static std::unordered_set<Decal*> decals;

	HDData::AnimationCurve curve;
};
