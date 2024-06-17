#include "TriggerBoxCollider.h"
#include "PhysicsSystem.h"
#include "GraphicsObjFactory.h"

HDData::TriggerBoxCollider::TriggerBoxCollider()
	: _width(1.0f), _height(1.0f), _depth(1.0f)
{
	_cubeDebugStruct = HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateCubePrimitive();
	_debugStruct = _cubeDebugStruct;
}

HDData::TriggerBoxCollider::TriggerBoxCollider(float width, float height, float depth)
	: _width(width), _height(height), _depth(depth)
{
	_cubeDebugStruct = HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateCubePrimitive();
	_debugStruct = _cubeDebugStruct;
}

float HDData::TriggerBoxCollider::GetHeight()
{
	return _height;
}

float HDData::TriggerBoxCollider::GetWidth()
{
	return _width;
}

float HDData::TriggerBoxCollider::GetDepth()
{
	return _depth;
}

void HDData::TriggerBoxCollider::SetHeight(float val)
{
	_height = val;
}

void HDData::TriggerBoxCollider::SetWidth(float val)
{
	_width = val;
}

void HDData::TriggerBoxCollider::SetDepth(float val)
{
	_depth = val;
}

void HDData::TriggerBoxCollider::Update()
{
	Matrix colWorld = Matrix::Identity;
	colWorld *= GetTransformMatrix();
	colWorld *= GetTransform()->GetWorldTM();
	_debugStruct->worldTM = colWorld;
	_debugStruct->color = { 0.0f,1.0f,0.0f,1.0f };

	_cubeDebugStruct->widthHeightDepth = { _width, _height, _depth };
}
