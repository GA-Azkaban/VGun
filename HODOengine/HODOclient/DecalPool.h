#pragma once
#include "../HODOengine/HODO_API.h"
#include "Decal.h"
#include <vector>

class DecalPool
{
public:
	~DecalPool();

	static DecalPool& Instance();

	Decal* SummonDecal(Vector3 targetPos);
	void RetrieveDecal(Decal* decal);

	HDData::GameObject* player;

private:
	DecalPool();
	static DecalPool* _instance;
	std::vector<Decal*> _decalList;
	HDData::Material* _decalWoodMaterial;
};
