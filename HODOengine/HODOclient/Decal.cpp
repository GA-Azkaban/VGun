#include "Decal.h"
#include "DecalPool.h"

Decal::Decal()
{
	_decalTimer.isRepeat = false;
	_decalTimer.duration = 0.5f;
	HDData::AnimationCurve curve;
	curve.AddKey(0.0f, 0.5f, [](float t) { return 1.0f; });
	curve.AddKey(0.5f, 1.0f, [](float t) {return -2.0f * t + 2.0f; });
	_decalTimer.onUpdate = [&](float progress)
		{
			float alpha = curve.Evaluate(progress);
			_meshRenderer->SetAlbedoColor(255, 255, 255, alpha * 255);
		};
	_decalTimer.onExpiration = [this]()
		{
			DecalPool::Instance()->Retrieve(this);
		};
	decals.insert(this);
}

Decal::~Decal()
{
	decals.erase(this);
}

void Decal::CreateDecal()
{
	if (!DecalPool::Instance())
	{
		auto decalPool = API::CreateObject(API::GetCurrentScene(), "DecalPool");
		decalPool->AddComponent<DecalPool>();
	}
	auto decal = API::CreateObject(API::GetCurrentScene());
	auto decalComp = decal->AddComponent<Decal>();
	decalComp->_meshRenderer = decal->AddComponent<HDData::MeshRenderer>();
	decalComp->_meshRenderer->LoadMesh("primitiveQuad");
	HDEngine::MaterialDesc matDesc;
	matDesc.materialName = "DecalMat";
	matDesc.albedo = 
	HDData::Material* mat = API::CreateMaterial(matDesc);
	decalComp->_meshRenderer->LoadMaterial(mat);
}

void Decal::RemoveAllDecals()
{
	for (auto each : decals)
	{
		API::GetCurrentScene()->DestroyObject(each->GetGameObject());
	}
}

void Decal::Update()
{
	_decalTimer.Update();
}

