#include "DecalPool.h"

DecalPool* DecalPool::_instance = nullptr;

DecalPool::DecalPool()
{
	HDEngine::MaterialDesc decalWoodMat;
	decalWoodMat.materialName = "BulletHoleWoodMat";
	decalWoodMat.color = { 46, 41, 36, 255 };
	decalWoodMat.normalMap = "T_Wood_08_N.png";
	decalWoodMat.mask = "T_Wood_08_M.png";

	_decalWoodMaterial = API::CreateMaterial(decalWoodMat);
}

DecalPool::~DecalPool()
{
	for (auto decal : _decalList)
	{
		delete decal;
		decal = nullptr;
	}
	_decalList.clear();
}

DecalPool& DecalPool::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new DecalPool();
	}
	return *_instance;
}

Decal* DecalPool::SummonDecal(Vector3 targetPos)
{
	if (_decalList.empty())
	{
		for (int i = 0; i < 10; ++i)
		{
			auto newDecal = Decal::CreateDecal();
			_decalList.push_back(newDecal);
		}
	}

	Decal* decal = _decalList.back();
	decal->GetGameObject()->SetSelfActive(true);
	auto decalRenderer = decal->GetGameObject()->GetComponent<HDData::MeshRenderer>();
	decalRenderer->LoadMaterial(_decalWoodMaterial);
	decalRenderer->SetAlbedoColor(255, 255, 255, 255);
	decal->SetEffectOn(targetPos);
	_decalList.pop_back();
	return decal;
}

void DecalPool::RetrieveDecal(Decal* decal)
{
	decal->GetGameObject()->SetSelfActive(false);
	_decalList.push_back(decal);
}
