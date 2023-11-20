#pragma once
#include "dllExporter.h"
#include "Component.h"
#include "../HODOmath/HODOmath.h"

/// <summary>
/// Transform�� ��� GameObject�� ���������� ������ �ִ� ������Ʈ�Դϴ�.
/// Transform�� ������Ʈ�� position, rotation, scale ������ ������ �ֽ��ϴ�.
/// </summary>

namespace hodoData
{
	class GameObject;
	class HODO_API Transform : public Component
	{
	public:		
		Transform();
		HDMaths::HDFLOAT3 GetWorldPosition() const;
		HDMaths::HDQuaternion GetWorldRotation() const;
		HDMaths::HDFLOAT3 GetWorldScale() const;
		HDMaths::HDFLOAT3 GetLocalPosition() const;
		HDMaths::HDQuaternion GetLocalRotation() const;
		HDMaths::HDFLOAT3 GetLocalScale() const;
		HDMaths::HDFLOAT4X4 GetWorldTM() const;
		HDMaths::HDFLOAT4X4 GetLocalTM() const;
		void SetWorldPosition(const HDMaths::HDFLOAT3& position);
		void SetWorldRotation(const HDMaths::HDQuaternion& rotation);
		void SetWorldScale(const HDMaths::HDFLOAT3& scale);
		void SetLocalPosition(const HDMaths::HDFLOAT3& position);
		void SetLocalRotation(const HDMaths::HDQuaternion& rotation);
		void SetLocalScale(const HDMaths::HDFLOAT3& scale);

	private:
		HDMaths::HDFLOAT3 _position;	// Local position of gameobject
		HDMaths::HDQuaternion _rotation;	// Local rotation of gameobject
		HDMaths::HDFLOAT3 _scale;	// Local scale of gameobject
	};
}

