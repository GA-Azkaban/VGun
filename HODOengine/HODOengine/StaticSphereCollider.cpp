#include "StaticSphereCollider.h"
#include "PhysicsSystem.h"
#include "GraphicsObjFactory.h"

namespace HDData
{

	StaticSphereCollider::StaticSphereCollider()
		: _radius(0.5f)
	{
		_sphereDebugStruct = HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateSpherePrimitive();
		_debugStruct = _sphereDebugStruct;
	}

	StaticSphereCollider::StaticSphereCollider(float radius)
		: _radius(radius)
	{
		_sphereDebugStruct = HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateSpherePrimitive();
		_debugStruct = _sphereDebugStruct;
	}

	float StaticSphereCollider::GetRadius()
	{
		return _radius;
	}

	void StaticSphereCollider::SetRadius(float radius)
	{
		_radius = radius;
	}

	void StaticSphereCollider::Update()
	{
		Matrix colWorld = Matrix::Identity;
		colWorld *= GetTransformMatrix();
		colWorld *= GetTransform()->GetWorldTM();
		colWorld.m[0][0] *= _radius;
		colWorld.m[1][1] *= _radius;
		colWorld.m[2][2] *= _radius;
		_debugStruct->worldTM = colWorld;
		_debugStruct->color = { 0.0f,1.0f,0.0f,1.0f };

		_sphereDebugStruct->diameter = _radius;
	}

}
