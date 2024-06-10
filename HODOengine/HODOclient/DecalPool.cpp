#include "DecalPool.h"
#include "Decal.h"

DecalPool* DecalPool::instance = nullptr;

DecalPool::DecalPool()
{
	instance = this;
}

DecalPool* DecalPool::Instance()
{
	return instance;
}

Decal* DecalPool::SummonDecal(Vector3 position)
{
	if (decalList.empty())
	{		
		Decal::CreateDecal();	
	}

	auto decal = decalList[decalList.size() - 1];
	decal->GetGameObject()->GetTransform()->SetPosition(position);
	decal->GetGameObject()->SetSelfActive(true);
	decalList.pop_back();
	return decal;
}

void DecalPool::Retrieve(Decal* decal)
{
	if (decal != nullptr)
	{
		decal->GetGameObject()->SetSelfActive(false);
		decalList.push_back(decal);
	}
}
