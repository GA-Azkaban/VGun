#pragma once
#include "Component.h"
#include "dllExporter.h"
#include "../HODOmath/HODOmath.h"

///
/// ���� ������Ʈ���� ���� ū ���̽� Ŭ����.
/// �ݶ��̴����� Transform�� ���õ� �������� �ٷ��.
/// 

namespace HDData
{
	class HODO_API Collider : public Component
	{
	public:
		Collider();

	public:
		virtual void UpdateToPhysics() abstract;

	public:
		void SetPositionOffset(HDMath::HDFLOAT3 pos);
		void SetRotationOffset(float x, float y, float z);
		void SetScaleOffset(HDMath::HDFLOAT3 sca);

		HDMath::HDFLOAT3 GetPositionOffset();
		HDMath::HDQuaternion GetRotationOffset();
		HDMath::HDFLOAT3 GetScaleOffset();

		HDMath::HDFLOAT4X4 GetTranslateMatrix();
		HDMath::HDFLOAT4X4 GetRotationMatrix();
		HDMath::HDFLOAT4X4 GetScaleMatrix();

		HDMath::HDFLOAT4X4 GetTransformMatrix();

	protected:
		HDMath::HDFLOAT3 _positionOffset;
		HDMath::HDQuaternion _rotationOffset;
		HDMath::HDFLOAT3 _scaleOffset;

	public:
		void Setflag(int flag);
		int GetFlag();

	protected:
		int _flag;
	};
}


