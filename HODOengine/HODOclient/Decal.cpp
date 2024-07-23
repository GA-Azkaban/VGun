#include "Decal.h"
#include "DecalPool.h"

Decal::Decal()
{
	_curve.AddKey(0.0f, 0.5f, [](float t) { return 1.0f; });
	_curve.AddKey(0.5f, 1.0f, [](float t) { return -1.6f * t + 1.8f; });

	_decalTimer.isRepeat = false;
	_decalTimer.duration = 1.0f;
	_decalTimer.onUpdate = [&](float progress)
	{
		// 알파 빼기
		float value = _curve.Evaluate(progress);
		int valueInt = static_cast<int>(value * 255);
		_decalMesh->SetAlbedoColor(255, 255, 255, valueInt);
	};
	_decalTimer.onExpiration = [&]()
	{
		DecalPool::Instance().RetrieveDecal(this);
	};
}

Decal* Decal::CreateDecal()
{
	auto obj = API::CreateObject(API::GetCurrentScene());
	obj->GetTransform()->SetScale(0.1f, 0.1f, 0.1f);
	auto comp = obj->AddComponent<Decal>();
	comp->_decalMesh = obj->AddComponent<HDData::MeshRenderer>();
	comp->_decalMesh->LoadMesh("primitiveQuad");
	comp->_decalMesh->SetShadowActive(false);
	comp->GetGameObject()->SetSelfActive(false);
	return comp;
}

void Decal::Update()
{
	_decalTimer.Update();
}

void Decal::SetEffectOn(Vector3 targetPos)
{
	_targetPos = targetPos;
	Vector3 toCamera = API::GetCurrenSceneMainCamera()->GetTransform()->GetPosition() - targetPos;
	toCamera.Normalize();
	toCamera *= 0.01f;
	GetGameObject()->GetTransform()->SetPosition(targetPos + toCamera);
	_decalTimer.Start();

	// 카메라 방향으로 회전
	Vector4 cameraRotation = API::GetCurrenSceneMainCamera()->GetTransform()->GetRotation();
	GetGameObject()->GetTransform()->SetRotation(cameraRotation);

}
