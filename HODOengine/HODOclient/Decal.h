#pragma once
#include "../HODOengine/HODO_API.h"
#include "Timer.h"
#include <unordered_set>

class Decal : public HDData::Script
{
public:
	static void CreateDecal();
	static void RemoveAllDecals();

protected:
	virtual void OnEnable() override;
	virtual void Update() override;

private:
	Decal();
	~Decal();
	Timer _decalTimer;
	HDData::MeshRenderer* _meshRenderer;

	static std::unordered_set<Decal*> decals;
};
