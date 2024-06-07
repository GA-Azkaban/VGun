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
	return decal;
}

void DecalPool::Retrieve(Decal* decal)
{

}
