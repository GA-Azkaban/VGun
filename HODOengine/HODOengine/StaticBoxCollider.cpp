#include "StaticBoxCollider.h"

hodoData::StaticBoxCollider::StaticBoxCollider()
	: _width(0.f), _height(0.f), _depth(0.f)
{

}

void hodoData::StaticBoxCollider::Start()
{

}

void hodoData::StaticBoxCollider::Update()
{

}

float hodoData::StaticBoxCollider::GetHeight()
{
	return _height;
}

float hodoData::StaticBoxCollider::GetWidth()
{
	return _width;
}

float hodoData::StaticBoxCollider::GetDepth()
{
	return _depth;
}

void hodoData::StaticBoxCollider::SetHeight(float val)
{
	_height = val;
}

void hodoData::StaticBoxCollider::SetWidth(float val)
{
	_width = val;
}

void hodoData::StaticBoxCollider::SetDepth(float val)
{
	_depth = val;
}
