#pragma once
#include "../HODOengine/HODO_API.h"
#include <vector>

class Decal;

class DecalPool : public HDData::Script
{
public:
	DecalPool();
	static DecalPool* Instance();
	Decal* SummonDecal(Vector3 position);
	void Retrieve(Decal* decal);

private:
	static DecalPool* instance;
	std::vector<Decal*> decalList;
};
