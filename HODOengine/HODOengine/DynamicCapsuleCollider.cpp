#include "DynamicCapsuleCollider.h"
#include "GameObject.h"
#include "GraphicsObjFactory.h"

HDData::DynamicCapsuleCollider::DynamicCapsuleCollider()
	: _radius(1.0f), _halfHeight(2.0f)
{
	_capsuleObject = HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateCapsulePrimitive();
	_debugStruct = _capsuleObject;
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

void HDData::DynamicCapsuleCollider::DrawDebug()
{
	Matrix colWorld = Matrix::Identity;
	colWorld *= GetTransformMatrix();
	colWorld *= GetTransform()->GetWorldTM();
	_debugStruct->worldTM = colWorld;
	_debugStruct->color = { 0.0f, 1.0f, 0.0f, 1.0f };

	_capsuleObject->height = _halfHeight * 2;
	_capsuleObject->diameter = _radius * 2;
}

float HDData::DynamicCapsuleCollider::GetRadius() const
{
	return _radius;
}

float HDData::DynamicCapsuleCollider::GetHalfHeight() const
{
	return _halfHeight;
}

void HDData::DynamicCapsuleCollider::SetRadius(float val)
{
	_radius = val;
}

void HDData::DynamicCapsuleCollider::SetHalfHeight(float val)
{
	_halfHeight = val;
}

