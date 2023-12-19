#include "DynamicBoxCollider.h"
#include "GameObject.h"

HDData::DynamicBoxCollider::DynamicBoxCollider()
{

}

void HDData::DynamicBoxCollider::SetVolume(float w, float h, float d)
{
	_width = w;
	_height = h;
	_depth = d;
}

float HDData::DynamicBoxCollider::GetWidth() const
{
	return _width * _scaleOffset.x * GetGameObject()->GetTransform()->GetWorldScale().x;
}

float HDData::DynamicBoxCollider::GetHeight() const
{
	return _height * _scaleOffset.x * GetGameObject()->GetTransform()->GetWorldScale().y;
}

float HDData::DynamicBoxCollider::GetDepth() const
{
	return _depth * _scaleOffset.x * GetGameObject()->GetTransform()->GetWorldScale().z;
}
