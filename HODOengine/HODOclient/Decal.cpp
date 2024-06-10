#include "Decal.h"
#include "DecalPool.h"

std::unordered_set<Decal*> Decal::decals;

Decal::Decal()
{
	_decalTimer.isRepeat = false;
	_decalTimer.duration = 500.0f;
	
	curve.AddKey(0.0f, 0.5f, [](float t) { return 1.0f; });
	curve.AddKey(0.5f, 1.0f, [](float t) { return -2.0f * t + 2.0f; });
	_decalTimer.onUpdate = [&](float progress)
		{
			float alpha = curve.Evaluate(progress);
			_decalRenderer->SetColor(255, 255, 255, alpha * 255);
		};
	//_decalTimer.onExpiration = [this]()
	//	{
	//		DecalPool::Instance()->Retrieve(this);
	//	};
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
	decalComp->_decalRenderer = decal->AddComponent<HDData::DecalRenderer>();
	HDEngine::MaterialDesc matDesc;
	matDesc.materialName = "DecalMat";
	matDesc.albedo = "FP_Black_A.png";
	HDData::Material* mat = API::CreateMaterial(matDesc);
	decalComp->_decalRenderer->LoadMaterial(mat);
	DecalPool::Instance()->Retrieve(decalComp);
}

void Decal::RemoveAllDecals()
{
	for (auto each : decals)
	{
		API::GetCurrentScene()->DestroyObject(each->GetGameObject());
	}
}

void Decal::OnEnable()
{
	_decalRenderer->SetColor(255, 255, 255, 255);
	_decalTimer.Start();
}

void Decal::Update()
{
	_decalTimer.Update();
}
