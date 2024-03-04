#include "DynamicCapsuleCollider.h"
#include "GameObject.h"
#include "GraphicsObjFactory.h"

HDData::DynamicCapsuleCollider::DynamicCapsuleCollider()
{
	_capsuleObject = HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateDebugObject();
	_capsuleObject->LoadMesh("capsule");
}

float HDData::DynamicCapsuleCollider::GetWidth() const
{
	//return (_radius + _halfHeight) * 2 * _scaleOffset.y * GetGameObject()->GetTransform()->GetScale().y;
	return 1.0f * 2 * GetGameObject()->GetTransform()->GetScale().y;
}

float HDData::DynamicCapsuleCollider::GetHeight() const
{
	//return (_radius + _halfHeight) * 2 * _scaleOffset.x * GetGameObject()->GetTransform()->GetScale().x;
	return 4.0f * GetGameObject()->GetTransform()->GetScale().x;
}

float HDData::DynamicCapsuleCollider::GetDepth() const
{
	return 1.0f * 2 * GetGameObject()->GetTransform()->GetScale().z;
}

void HDData::DynamicCapsuleCollider::Update()
{
	_capsuleObject->SetWorldTM(GetGameObject()->GetTransform()->GetWorldTM());
}

void HDData::DynamicCapsuleCollider::DrawDebug()
{
	//Matrix colWorld = Matrix::Identity;
	//colWorld *= GetTransformMatrix();
	//colWorld *= GetTransform()->GetWorldTM();
	//_debugStruct->worldTM = colWorld;
	//_debugStruct->color = { 0.0f,1.0f,0.0f,1.0f };
	//
	//_capsuleDebugStruct->height = _halfHeight * 2;
	//_capsuleDebugStruct->diameter = _radius * 2;
}

void HDData::DynamicCapsuleCollider::OnEnable()
{
	_capsuleObject->SetActive(true);
}

void HDData::DynamicCapsuleCollider::OnDisable()
{
	_capsuleObject->SetActive(false);
}

void HDData::DynamicCapsuleCollider::SetFillModeSolid()
{
	_capsuleObject->SetFillModeSolid();
}

void HDData::DynamicCapsuleCollider::SetFillModeWireframe()
{
	_capsuleObject->SetFillModeWireframe();
}
