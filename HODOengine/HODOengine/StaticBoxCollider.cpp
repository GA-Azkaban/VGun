#include "StaticBoxCollider.h"
#include "PhysicsSystem.h"

HDData::StaticBoxCollider::StaticBoxCollider()
	: _width(1.0f), _height(1.0f), _depth(1.0f)
{
	//HDEngine::PhysicsSystem::Instance().CreateStaticBoxCollider(_width, _height, _depth);
}

HDData::StaticBoxCollider::StaticBoxCollider(float width, float height, float depth)
	: _width(width), _height(height), _depth(depth)
{
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
