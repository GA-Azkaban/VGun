#include "DynamicSphereCollider.h"
#include "GameObject.h"
#include "GraphicsObjFactory.h"

HDData::DynamicSphereCollider::DynamicSphereCollider()
	: _radius(1.0f)
{
	_debugStruct = HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateSpherePrimitive();
}

float HDData::DynamicSphereCollider::GetWidth() const
{
	return _radius * 2 * _scaleOffset.x * GetGameObject()->GetTransform()->GetScale().x;
}

float HDData::DynamicSphereCollider::GetHeight() const
{
	return _radius * 2 * _scaleOffset.y * GetGameObject()->GetTransform()->GetScale().y;
}

float HDData::DynamicSphereCollider::GetDepth() const
{
	return _radius * 2 * _scaleOffset.z * GetGameObject()->GetTransform()->GetScale().z;
}

float HDData::DynamicSphereCollider::GetRadius() const
{
	return _radius;
}

void HDData::DynamicSphereCollider::SetRadius(float radius)
{
	_radius = radius;
}

void HDData::DynamicSphereCollider::DrawDebug()
{
	_debugStruct->worldTM = GetTransform()->GetWorldTM();
	_debugStruct->color = { 0.0f,1.0f,0.0f,1.0f };
}
