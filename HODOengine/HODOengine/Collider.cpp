#include "Collider.h"
#include "DebugSystem.h"
#include "RenderSystem.h"

namespace HDData
{

	Collider::Collider()
	{
		HDEngine::RenderSystem::Instance().PushCollider(this);
	}

	void Collider::SetPositionOffset(Vector3 pos)
	{
		_positionOffset = pos;
	}

	void Collider::SetRotationOffset(float x, float y, float z)
	{

	}

	void Collider::SetScaleOffset(Vector3 sca)
	{
		_scaleOffset = sca;
	}

	Vector3 Collider::GetPositionOffset()
	{
		return _positionOffset;
	}

	Quaternion Collider::GetRotationOffset()
	{
		return _rotationOffset;
	}

	Vector3 Collider::GetScaleOffset()
	{
		return _scaleOffset;
	}

	Matrix Collider::GetTranslateMatrix()
	{
		Matrix translateMatrix =
		{
			1,					0,					0,					0,
			0,					1,					0,					0,
			0,					0,					1,					0,
			_positionOffset.x,	_positionOffset.y,	_positionOffset.z,	1
		};

		return translateMatrix;
	}

	Matrix Collider::GetRotationMatrix()
	{
		Matrix scaleMatrix =
		{
			_scaleOffset.x,		0,					0,					0,
			0,					_scaleOffset.y,		0,					0,
			0,					0,					_scaleOffset.z,		0,
			0,					0,					0,					1
		};

		return scaleMatrix;
	}

	Matrix Collider::GetScaleMatrix()
	{
		Matrix rotationMatrix =
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

	Matrix Collider::GetTransformMatrix()
	{
		return GetScaleMatrix() * GetRotationMatrix() * GetTranslateMatrix();
	}

	void Collider::Setflag(int flag)
	{
		_flag = flag;
	}

	int Collider::GetFlag()
	{
		return _flag;
	}

}
