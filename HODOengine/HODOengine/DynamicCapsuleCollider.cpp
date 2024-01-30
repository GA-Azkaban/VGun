#include "DynamicCapsuleCollider.h"
#include "GameObject.h"
#include "GraphicsObjFactory.h"

HDData::DynamicCapsuleCollider::DynamicCapsuleCollider()
	: _radius(1.0f), _halfHeight(1.0f)
{
	_debugStruct = nullptr; // HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateCylinderPrimitive();
}

float HDData::DynamicCapsuleCollider::GetWidth() const
{
	return (_radius + _halfHeight) * 2 * _scaleOffset.y * GetGameObject()->GetTransform()->GetScale().y;
}

float HDData::DynamicCapsuleCollider::GetHeight() const
{
	return (_radius + _halfHeight) * 2 * _scaleOffset.x * GetGameObject()->GetTransform()->GetScale().x;
}

float HDData::DynamicCapsuleCollider::GetDepth() const
{
	return (_radius + _halfHeight) * 2 * _scaleOffset.z * GetGameObject()->GetTransform()->GetScale().z;
}

float HDData::DynamicCapsuleCollider::GetRadius() const
{
	return _radius;
}

float HDData::DynamicCapsuleCollider::GetHalfHeight() const
{
	return _halfHeight;
}

void HDData::DynamicCapsuleCollider::SetCapsuleInfo(float radius, float halfHeight)
{
	_radius = radius;
	_halfHeight = halfHeight;
}

void HDData::DynamicCapsuleCollider::SetRadius(float rad)
{
	_radius = rad;
}

void HDData::DynamicCapsuleCollider::SetHalfHeight(float val)
{
	_halfHeight = val;
}

void HDData::DynamicCapsuleCollider::DrawDebug()
{
	_debugStruct->worldTM = GetTransform()->GetWorldTM();
	_debugStruct->color = { 0.0f,1.0f,0.0f,1.0f };
}
