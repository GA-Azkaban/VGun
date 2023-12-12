#pragma once
#include "dllExporter.h"
#include "Component.h"
#include "../HODOmath/HODOmath.h"

/// <summary>
/// Transform은 모든 GameObject가 공통적으로 가지고 있는 컴포넌트입니다.
/// Transform은 오브젝트의 position, rotation, scale 정보를 가지고 있습니다.
/// </summary>

namespace HDData
{
	class GameObject;
	class HODO_API Transform : public Component
	{
	public:		
		Transform();
		HDMath::HDFLOAT3 GetWorldPosition() const;
		HDMath::HDQuaternion GetWorldRotation() const;
		HDMath::HDFLOAT3 GetWorldScale() const;
		HDMath::HDFLOAT3 GetLocalPosition() const;
		HDMath::HDQuaternion GetLocalRotation() const;
		HDMath::HDFLOAT3 GetLocalScale() const;
		HDMath::HDFLOAT4X4 GetWorldTM() const;
		HDMath::HDFLOAT4X4 GetLocalTM() const;
		void SetWorldPosition(const HDMath::HDFLOAT3& position);
		void SetWorldRotation(const HDMath::HDQuaternion& rotation);
		void SetWorldScale(const HDMath::HDFLOAT3& scale);
		void SetLocalPosition(const HDMath::HDFLOAT3& position);
		void SetLocalRotation(const HDMath::HDQuaternion& rotation);
		void SetLocalScale(const HDMath::HDFLOAT3& scale);

	private:
		HDMath::HDFLOAT3 _position;	// Local position of gameobject
		HDMath::HDQuaternion _rotation;	// Local rotation of gameobject
		HDMath::HDFLOAT3 _scale;	// Local scale of gameobject
	};
}

