#include "DynamicSphereCollider.h"
#include "GameObject.h"
#include "GraphicsObjFactory.h"

HDData::DynamicSphereCollider::DynamicSphereCollider()
	: _radius(0.5f)
{
	_sphereDebugStruct = HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateSpherePrimitive();
	_debugStruct = _sphereDebugStruct;
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
	Matrix colWorld = Matrix::Identity;
	colWorld *= GetTransformMatrix();
	colWorld *= GetTransform()->GetWorldTM();
	_debugStruct->worldTM = colWorld;
	_debugStruct->color = { 0.0f,1.0f,0.0f,1.0f };

	_sphereDebugStruct->diameter = _radius * 2;
}
