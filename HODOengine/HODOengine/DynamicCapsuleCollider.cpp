#include "DynamicCapsuleCollider.h"
#include "GameObject.h"
#include "GraphicsObjFactory.h"

HDData::DynamicCapsuleCollider::DynamicCapsuleCollider()
	: _radius(1.0f), _halfHeight(1.0f)
{
	_capsuleObject = HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateCapsulePrimitive();
	_debugStruct = _capsuleObject;
}

HDData::DynamicCapsuleCollider::DynamicCapsuleCollider(float rad, float halfHeight, int colFilterNum)
{
	_radius = rad;
	_halfHeight = halfHeight;

	_capsuleObject = HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateCapsulePrimitive();
	_debugStruct = _capsuleObject;

	_collisionFilterNum = colFilterNum;
}

float HDData::DynamicCapsuleCollider::GetWidth() const
{
	return (_radius) * 2 * _scaleOffset.x * GetGameObject()->GetTransform()->GetScale().x;
}

float HDData::DynamicCapsuleCollider::GetHeight() const
{
	return (_radius + _halfHeight * 2) * _scaleOffset.y * GetGameObject()->GetTransform()->GetScale().y;
}

float HDData::DynamicCapsuleCollider::GetDepth() const
{
	return (_radius) * 2 * _scaleOffset.z * GetGameObject()->GetTransform()->GetScale().z;
}

void HDData::DynamicCapsuleCollider::Update()
{
	Matrix colWorld = Matrix::Identity;
	colWorld *= GetTransformMatrix();
	colWorld._11 *= _radius;
	colWorld._22 *= _halfHeight;
	colWorld._33 *= _radius;
	colWorld *= GetTransform()->GetWorldTM();
	_debugStruct->worldTM = colWorld;
	_debugStruct->color = { 0.0f, 1.0f, 0.0f, 1.0f };

	_capsuleObject->height = _halfHeight * 2;
	_capsuleObject->diameter = _radius * 2;
}

float HDData::DynamicCapsuleCollider::GetRadius() const
{
	return _radius * _scaleOffset.x * GetGameObject()->GetTransform()->GetScale().x;
}

float HDData::DynamicCapsuleCollider::GetHalfHeight() const
{
	return _halfHeight * _scaleOffset.y * GetGameObject()->GetTransform()->GetScale().y;
}

void HDData::DynamicCapsuleCollider::SetRadius(float val)
{
	_radius = val;
}

void HDData::DynamicCapsuleCollider::SetHalfHeight(float val)
{
	_halfHeight = val;
}

