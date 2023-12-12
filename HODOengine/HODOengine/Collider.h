#pragma once
#include "Component.h"
#include "dllExporter.h"
#include "../HODOmath/HODOmath.h"

///
/// ���� ������Ʈ���� ���� ū ���̽� Ŭ����.
/// �ݶ��̴����� Transform�� ���õ� �������� �ٷ��.
/// 

namespace hodoData
{
	class HODO_API Collider : public Component
	{
	public:
		Collider();

	public:
		virtual void UpdateToPhysics() abstract;

	public:
		void SetPositionOffset(HDMaths::HDFLOAT3 pos);
		void SetRotationOffset(float x, float y, float z);
		void SetScaleOffset(HDMaths::HDFLOAT3 sca);

		HDMaths::HDFLOAT3 GetPositionOffset();
		HDMaths::HDQuaternion GetRotationOffset();
		HDMaths::HDFLOAT3 GetScaleOffset();

		HDMaths::HDFLOAT4X4 GetTranslateMatrix();
		HDMaths::HDFLOAT4X4 GetRotationMatrix();
		HDMaths::HDFLOAT4X4 GetScaleMatrix();

		HDMaths::HDFLOAT4X4 GetTransformMatrix();

	protected:
		HDMaths::HDFLOAT3 _positionOffset;
		HDMaths::HDQuaternion _rotationOffset;
		HDMaths::HDFLOAT3 _scaleOffset;

		// ������ �������� ���� �÷��� ��
		// Ŭ���̾�Ʈ���� ���� �����ϵ���......
	public:
		void Setflag(int flag);
		int GetFlag();

	protected:
		int _flag;
	};
}


