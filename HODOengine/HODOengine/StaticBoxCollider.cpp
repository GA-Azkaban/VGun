#include "StaticBoxCollider.h"
#include "PhysicsSystem.h"
#include "GraphicsObjFactory.h"

HDData::StaticBoxCollider::StaticBoxCollider()
	: _width(1.0f), _height(1.0f), _depth(1.0f)
{
	_debugStruct = nullptr; // HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateCubePrimitive();
	//HDEngine::PhysicsSystem::Instance().CreateStaticBoxCollider(_width, _height, _depth);
}

HDData::StaticBoxCollider::StaticBoxCollider(float width, float height, float depth)
	: _width(width), _height(height), _depth(depth)
{
	_debugStruct = nullptr; //HDEngine::GraphicsObjFactory::Instance().GetFactory()->CreateCubePrimitive();
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
	_debugStruct->worldTM = GetTransform()->GetWorldTM();
	_debugStruct->color = { 0.0f,1.0f,0.0f,1.0f };
}
