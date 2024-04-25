#include "StaticBoxCollider.h"
#include "PhysicsSystem.h"
#include "GraphicsObjFactory.h"

HDData::StaticBoxCollider::StaticBoxCollider()
	: _width(1.0f), _height(1.0f), _depth(1.0f)
	//: _width(0.01f), _height(0.01f), _depth(0.01f)
{
	_cubeDebugStruct = HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateCubePrimitive();
	_debugStruct = _cubeDebugStruct;
	//HDEngine::PhysicsSystem::Instance().CreateStaticBoxCollider(_width, _height, _depth);
}

HDData::StaticBoxCollider::StaticBoxCollider(float width, float height, float depth)
	: _width(width), _height(height), _depth(depth)
{
	_debugStruct = HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateCubePrimitive();
	//HDEngine::PhysicsSystem::Instance().CreateStaticBoxCollider(_width, _height, _depth);
}

float HDData::StaticBoxCollider::GetHeight()
{
	return _height;
}

float HDData::StaticBoxCollider::GetWidth()
{
	return _width;
}

float HDData::StaticBoxCollider::GetDepth()
{
	return _depth;
}

void HDData::StaticBoxCollider::SetHeight(float val)
{
	_height = val;
}

void HDData::StaticBoxCollider::SetWidth(float val)
{
	_width = val;
}

void HDData::StaticBoxCollider::SetDepth(float val)
{
	_depth = val;
}

void HDData::StaticBoxCollider::DrawDebug()
{
	Matrix colWorld = Matrix::Identity;
	colWorld *= GetTransformMatrix();
	colWorld *= GetTransform()->GetWorldTM();
	colWorld.m[0][0] *= _width;
	colWorld.m[1][1] *= _height;
	colWorld.m[2][2] *= _depth;
	_debugStruct->worldTM = colWorld;
	_debugStruct->color = { 0.0f,1.0f,0.0f,1.0f };

	_cubeDebugStruct->widthHeightDepth = { _width, _height, _depth };
}
