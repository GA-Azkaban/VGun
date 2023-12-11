#include "Collider.h"
#include "DebugSystem.h"

hodoData::Collider::Collider()
{
	HDMaths::HDFLOAT4 color = { 1.f, 0.f, 0.f, 0.f };
	hodoEngine::DebugSystem::Instance().DrawColliderDebug(GetGameObject(), color);
}

void hodoData::Collider::SetPositionOffset(HDMaths::HDFLOAT3 pos)
{
	_positionOffset = pos;
}

void hodoData::Collider::SetRotationOffset(float x, float y, float z)
{
	
}

void hodoData::Collider::SetScaleOffset(HDMaths::HDFLOAT3 sca)
{
	_scaleOffset = sca;
}

HDMaths::HDFLOAT3 hodoData::Collider::GetPositionOffset()
{
	return _positionOffset;
}

HDMaths::HDQuaternion hodoData::Collider::GetRotationOffset()
{
	return _rotationOffset;
}

HDMaths::HDFLOAT3 hodoData::Collider::GetScaleOffset()
{
	return _scaleOffset;
}

HDMaths::HDFLOAT4X4 hodoData::Collider::GetTranslateMatrix()
{
	HDMaths::HDFLOAT4X4 translateMatrix =
	{
		1,					0,					0,					0,
		0,					1,					0,					0,
		0,					0,					1,					0,
		_positionOffset.x,	_positionOffset.y,	_positionOffset.z,	1
	};

	return translateMatrix;
}

HDMaths::HDFLOAT4X4 hodoData::Collider::GetRotationMatrix()
{
	HDMaths::HDFLOAT4X4 scaleMatrix =
	{
		_scaleOffset.x,		0,					0,					0,
		0,					_scaleOffset.y,		0,					0,
		0,					0,					_scaleOffset.z,		0,
		0,					0,					0,					1
	};

	return scaleMatrix;
}

HDMaths::HDFLOAT4X4 hodoData::Collider::GetScaleMatrix()
{
	HDMaths::HDFLOAT4X4 rotationMatrix =
	{
		1.0f - 2.0f * (_rotationOffset.y * _rotationOffset.y + _rotationOffset.z * _rotationOffset.z),
		2.0f * (_rotationOffset.x * _rotationOffset.y + _rotationOffset.z * _rotationOffset.w),
		2.0f * (_rotationOffset.x * _rotationOffset.z - _rotationOffset.y * _rotationOffset.w),
		0,

		2.0f * (_rotationOffset.x * _rotationOffset.y - _rotationOffset.z * _rotationOffset.w),
		1.0f - 2.0f * (_rotationOffset.x * _rotationOffset.x + _rotationOffset.z * _rotationOffset.z),
		2.0f * (_rotationOffset.y * _rotationOffset.z + _rotationOffset.x * _rotationOffset.w),
		0,

		2.0f * (_rotationOffset.x * _rotationOffset.z + _rotationOffset.y * _rotationOffset.w),
		2.0f * (_rotationOffset.y * _rotationOffset.z - _rotationOffset.x * _rotationOffset.w),
		1.0f - 2.0f * (_rotationOffset.x * _rotationOffset.x + _rotationOffset.y * _rotationOffset.y),
		0,

		0,
		0,
		0,
		1
	};

	return rotationMatrix;
}

HDMaths::HDFLOAT4X4 hodoData::Collider::GetTransformMatrix()
{
	return GetScaleMatrix() * GetRotationMatrix() * GetTranslateMatrix();
}

void hodoData::Collider::SetFlag(int flag)
{
	_flag = flag;
}

int hodoData::Collider::GetFlag()
{
	return _flag;
}
