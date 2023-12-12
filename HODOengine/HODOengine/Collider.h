#pragma once
#include "Component.h"
#include "dllExporter.h"
#include "../HODOmath/HODOmath.h"

///
/// 물리 컴포넌트들의 가장 큰 베이스 클래스.
/// 콜라이더들의 Transform과 관련된 정보들을 다룬다.
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

		// 종류별 렌더링을 위한 플래그 값
		// 클라이언트에서 설정 가능하도록......
	public:
		void SetFlag(int flag);
		int GetFlag();

	protected:
		int _flag;
	};
}


