#include "ParticleSphereCollider.h"
#include "GameObject.h"
#include "GraphicsObjFactory.h"

HDData::ParticleSphereCollider::ParticleSphereCollider()
	: _radius(0.01f)
{
	_sphereDebugStruct = HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateSpherePrimitive();
	_debugStruct = _sphereDebugStruct;
}

float HDData::ParticleSphereCollider::GetRadius() const
{
	return _radius;
}

void HDData::ParticleSphereCollider::SetRadius(float radius)
{
	_radius = radius;
}

void HDData::ParticleSphereCollider::CheckTimer(float deltaTime)
{
	if (_duration > 0.0f)
	{
		//_duration -= deltaTime;
	}
	else
	{
		this->SetGlobalPosition(Vector3(0.0f, -5.0f, 0.0f));
	}
}

void HDData::ParticleSphereCollider::SetTimerActive()
{
	_duration = 1.0f;
}

void HDData::ParticleSphereCollider::DrawDebug()
{
	Matrix colWorld = Matrix::Identity;
	colWorld *= GetTransformMatrix();
	colWorld *= GetTransform()->GetWorldTM();
	_debugStruct->worldTM = colWorld;
	_debugStruct->color = { 1.0f,0.0f,0.0f,1.0f };

	_sphereDebugStruct->diameter = _radius * 2;
}
