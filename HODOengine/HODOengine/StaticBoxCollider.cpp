#include "StaticBoxCollider.h"

HDData::StaticBoxCollider::StaticBoxCollider()
	: _width(0.f), _height(0.f), _depth(0.f)
{

}

void HDData::StaticBoxCollider::Start()
{

}

void HDData::StaticBoxCollider::Update()
{

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
