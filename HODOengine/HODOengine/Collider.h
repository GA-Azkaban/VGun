#pragma once
#include "Component.h"
#include "dllExporter.h"
#include "../HODOmath/HODOmath.h"

///
/// 물리 컴포넌트들의 가장 큰 베이스 클래스.
/// 콜라이더들의 Transform과 관련된 정보들을 다룬다.
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

		// 종류별 렌더링을 위한 플래그 값
		// 클라이언트에서 설정 가능하도록......
	public:
		void Setflag(int flag);
		int GetFlag();

	protected:
		int _flag;
	};
}


