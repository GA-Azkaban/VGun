#include "DynamicSphereCollider.h"
#include "GameObject.h"
#include "GraphicsObjFactory.h"

HDData::DynamicSphereCollider::DynamicSphereCollider()
	: _radius(10.0f)
{
	_sphereDebugStruct = HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateSpherePrimitive();
	_debugStruct = _sphereDebugStruct;
}

HDData::DynamicSphereCollider::DynamicSphereCollider(float rad, int colFilterNum)
	: _radius(rad)
{
	_sphereDebugStruct = HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateSpherePrimitive();
	_debugStruct = _sphereDebugStruct;

	_collisionFilterNum = colFilterNum;
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

void HDData::DynamicSphereCollider::Update()
{
	Matrix colWorld = Matrix::Identity;
	colWorld *= GetTransformMatrix();
	colWorld *= GetTransform()->GetWorldTM();
	_debugStruct->worldTM = colWorld;
	_debugStruct->color = { 0.0f,1.0f,0.0f,1.0f };

	_sphereDebugStruct->diameter = _radius * 2;

	//Transform* transform = GetTransform();
	//Vector3 scale = transform->GetScale();
	//Vector3 pos = transform->GetPosition();
	//
	//Matrix rotMat = XMMatrixRotationQuaternion(transform->GetRotation());
	//Matrix scaleMat = { scale.x, 0, 0, 0,  0, scale.y, 0, 0,  0, 0, scale.z, 0,  0, 0, 0, 1 };
	//Matrix transformMat = rotMat * scaleMat;
	//transformMat.m[3][0] = pos.x;
	//transformMat.m[3][1] = pos.y;
	//transformMat.m[3][2] = pos.z;
	//transformMat.m[3][3] = 1;
	//
	//_debugStruct->worldTM = transformMat;
}
