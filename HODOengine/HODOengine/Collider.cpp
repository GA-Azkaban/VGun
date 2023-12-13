#include "Collider.h"
#include "DebugSystem.h"

HDData::Collider::Collider()
{
	HDMath::HDFLOAT4 color = { 1.f, 0.f, 0.f, 0.f };
	HDEngine::DebugSystem::Instance().DrawColliderDebug(GetGameObject(), color);
}

void HDData::Collider::SetPositionOffset(HDMath::HDFLOAT3 pos)
{
	_positionOffset = pos;
}

void HDData::Collider::SetRotationOffset(float x, float y, float z)
{
	
}

void HDData::Collider::SetScaleOffset(HDMath::HDFLOAT3 sca)
{
	_scaleOffset = sca;
}

HDMath::HDFLOAT3 HDData::Collider::GetPositionOffset()
{
	return _positionOffset;
}

HDMath::HDQuaternion HDData::Collider::GetRotationOffset()
{
	return _rotationOffset;
}

HDMath::HDFLOAT3 HDData::Collider::GetScaleOffset()
{
	return _scaleOffset;
}

HDMath::HDFLOAT4X4 HDData::Collider::GetTranslateMatrix()
{
	HDMath::HDFLOAT4X4 translateMatrix =
	{
		1,					0,					0,					0,
		0,					1,					0,					0,
		0,					0,					1,					0,
		_positionOffset.x,	_positionOffset.y,	_positionOffset.z,	1
	};

	return translateMatrix;
}

HDMath::HDFLOAT4X4 HDData::Collider::GetRotationMatrix()
{
	HDMath::HDFLOAT4X4 scaleMatrix =
	{
		_scaleOffset.x,		0,					0,					0,
		0,					_scaleOffset.y,		0,					0,
		0,					0,					_scaleOffset.z,		0,
		0,					0,					0,					1
	};

	return scaleMatrix;
}

HDMath::HDFLOAT4X4 HDData::Collider::GetScaleMatrix()
{
	HDMath::HDFLOAT4X4 rotationMatrix =
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

HDMath::HDFLOAT4X4 HDData::Collider::GetTransformMatrix()
{
	return GetScaleMatrix() * GetRotationMatrix() * GetTranslateMatrix();
}

void HDData::Collider::SetFlag(int flag)
{
	_flag = flag;
}

int HDData::Collider::GetFlag()
{
	return _flag;
}
